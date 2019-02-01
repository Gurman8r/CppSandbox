#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Texture::Texture()
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(false)
		, m_srgb		(false)
		, m_repeated	(false)
		, m_mipmapped	(false)
	{
	}

	Texture::Texture(const Texture & copy)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(copy.m_smooth)
		, m_srgb		(copy.m_srgb)
		, m_repeated	(copy.m_repeated)
		, m_mipmapped	(false)
	{
		if (copy)
		{
			if (create(copy.size()[0], copy.size()[1]))
			{
				update(copy);

				OpenGL::flush();
			}
			else
			{
				Debug::LogError("Failed to copy texture, failed to create new texture");
			}
		}
	}

	Texture::~Texture()
	{
		cleanup();
	}


	bool Texture::cleanup()
	{
		if ((*this))
		{
			OpenGL::deleteTextures(1, (*this));
			return true;
		}
		return false;
	}

	bool Texture::loadFromFile(const String & filename)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return loadFromImage(value, IntRect(vec2i::Zero, value.size()));
	}

	bool Texture::loadFromImage(const Image & image, const IntRect & area)
	{
		// Image size
		const int32_t w = (int32_t)image.width();
		const int32_t h = (int32_t)image.height();

		// Load the entire image if the source area is empty or the whole image
		if ((area.width() == 0 || area.height() == 0) || (
			area.left() <= 0 && 
			area.top() <= 0 && 
			area.width() >= w && 
			area.height() >= h))
		{
			if (create(image.size()[0], image.size()[1]))
			{
				update(image);

				return true;
			}
			return false;
		}
		else
		{
			// Adjust the rectangle to the size of the image
			IntRect rect = area;
			if (rect.left()  < 0) rect.left(0);
			if (rect.top()	 < 0) rect.top(0);
			if (rect.right() > w) rect.right(w);
			if (rect.bot()	 > h) rect.bot(h);

			// Create the texture and upload the pixels
			if (create(rect.width(), rect.height()))
			{
				// Copy the pixels to the texture, row by row
				const uint8_t * pixels = image.ptr() + 4 * (rect.left() + (w * rect.top()));

				Texture::bind(this);

				for (int32_t i = 0; i < rect.height(); i++)
				{
					OpenGL::texSubImage2D(
						GL::Texture2D,
						0, 
						0, i, rect.width(), 1, 
						GL::RGBA, 
						GL::UnsignedByte, 
						pixels);

					pixels += 4 * w;
				}

				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMinFilter,
					(m_smooth
						? GL::Linear
						: GL::Nearest));

				m_mipmapped = false;

				// Force an OpenGL flush, so that the texture will appear updated
				OpenGL::flush();

				return true;
			}
			return false;
		}
	}
	
	
	bool Texture::update(const Texture & texture)
	{
		return update(texture.copyToImage());
	}
	
	bool Texture::update(const Image & image)
	{
		return update(image.ptr(), image.size()[0], image.size()[1], 0, 0);
	}
	
	bool Texture::update(
		const uint8_t * pixels, 
		uint32_t width, 
		uint32_t height, 
		uint32_t x, 
		uint32_t y)
	{
		if ((*this) && pixels)
		{
			Texture::bind(this);

			OpenGL::texSubImage2D(
				GL::Texture2D,
				0,
				x, y, width, height,
				GL::RGBA,
				GL::UnsignedByte,
				pixels);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			m_mipmapped = false;

			Texture::bind(NULL);

			OpenGL::flush();

			return true;
		}
		return false;
	}


	bool Texture::create(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			return Debug::LogError("Failed creating texture, invalid size {0}", m_size);
		}

		uint32_t maxSize = OpenGL::getMaxTextureSize();
		vec2u actualSize(
			OpenGL::getValidTextureSize(width),
			OpenGL::getValidTextureSize(height));
		if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
		{
			return Debug::LogError(
				"Failed creating texture: "
				"Internal size is too high {0} "
				"Maximum is {1}",
				actualSize,
				vec2u(maxSize));
		}

		m_actualSize = actualSize;
		m_size = vec2u(width, height);

		if (!(*this) && (get_ref() = OpenGL::genTextures(1)))
		{
			
		}

		if (!m_repeated && !OpenGL::edgeClampAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::LogWarning(
					"OpenGL extension SGIS_texture_edge_clamp unavailable\n"
					"Artifacts may occur along texture edges\n"
					"Ensure that hardware acceleration is enabled if available");
				warned = true;
			}
		}

		if (m_srgb && !OpenGL::textureSrgbAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::LogWarning(
					"OpenGL ES extension EXT_sRGB unavailable\n"
					"Automatic sRGB to linear conversion disabled");
				warned = true;
			}
			m_srgb = false;
		}

		m_mipmapped = false;

		// Initialize the texture
		Texture::bind(this);
		
		OpenGL::texImage2D(
			GL::Texture2D,
			0,
			(m_srgb
				? GL::SRGB8_Alpha8
				: GL::RGBA),
			m_actualSize[0], 
			m_actualSize[1], 
			0, 
			GL::RGBA, 
			GL::UnsignedByte, 
			NULL);
		
		OpenGL::texParameter(
			GL::Texture2D,
			GL::TexWrapS,
			(m_repeated
				? GL::Repeat
				: (OpenGL::edgeClampAvailable()
					? GL::ClampToEdge
					: GL::Clamp)));
		
		OpenGL::texParameter(
			GL::Texture2D,
			GL::TexWrapT, 
			(m_repeated
				? GL::Repeat
				: (OpenGL::edgeClampAvailable()
					? GL::ClampToEdge
					: GL::Clamp)));
		
		OpenGL::texParameter(
			GL::Texture2D, 
			GL::TexMagFilter,
			(m_smooth
				? GL::Linear
				: GL::Nearest));
		
		OpenGL::texParameter(
			GL::Texture2D, 
			GL::TexMinFilter, 
			(m_smooth
				? GL::Linear
				: GL::Nearest));
		
		Texture::bind(NULL);
		
		return true;
	}

	bool Texture::create(
		const uint8_t * pixels, 
		uint32_t	width, 
		uint32_t	height,
		GL::Format	colFmt,
		GL::Format	intFmt, 
		bool		smooth, 
		bool		repeat,
		bool		srgb,
		bool		mipmapped)
	{
		if (!(*this) && (get_ref() = OpenGL::genTextures(1)))
		{
			if (!width || !height)
			{
				return Debug::LogError("Failed creating texture, invalid size {0}",
						vec2u(width, height));
			}

			uint32_t maxSize = OpenGL::getMaxTextureSize();
			vec2u actualSize(
				OpenGL::getValidTextureSize(width),
				OpenGL::getValidTextureSize(height));
			if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
			{
				return Debug::LogError(
					"Failed creating texture: "
					"Internal size is too high {0} "
					"Maximum is {1}",
					actualSize,
					vec2u(maxSize));
			}

			m_size			= { width, height };
			m_actualSize	= actualSize;
			m_smooth		= smooth;
			m_srgb			= srgb;
			m_repeated		= repeat;
			m_mipmapped		= mipmapped;
		
			Texture::bind(this);

			OpenGL::texImage2D(
				GL::Texture2D,
				0,
				colFmt,
				m_size[0],
				m_size[1],
				0,
				intFmt,
				GL::UnsignedByte,
				pixels);

			OpenGL::texParameter( // TexWrapS
				GL::Texture2D,
				GL::TexWrapS,
					(m_repeated
						? GL::Repeat
						: (OpenGL::edgeClampAvailable()
							? GL::ClampToEdge
							: GL::Clamp)));

			OpenGL::texParameter( // TexWrapT
				GL::Texture2D,
				GL::TexWrapT,
					(m_repeated
						? GL::Repeat
						: (OpenGL::edgeClampAvailable()
							? GL::ClampToEdge
							: GL::Clamp)));

			OpenGL::texParameter( // TexMagFilter
				GL::Texture2D,
				GL::TexMagFilter,
					(m_smooth
						? GL::Linear
						: GL::Nearest));

			OpenGL::texParameter( // TexMinFilter
				GL::Texture2D,
				GL::TexMinFilter,
					(m_smooth
						? GL::Linear
						: GL::Nearest));

			Texture::bind(NULL);
			return true;
		}
		return false;
	}


	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_ref(),	other.get_ref());
		std::swap(m_size,		other.m_size);
		std::swap(m_actualSize,	other.m_actualSize);
		std::swap(m_smooth,		other.m_smooth);
		std::swap(m_srgb,		other.m_srgb);
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);

		return other;
	}

	Texture & Texture::setRepeated(bool value)
	{
		if ((*this) && (m_repeated != value))
		{
			if ((m_repeated = value) && !OpenGL::edgeClampAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::LogWarning(
						"OpenGL extension texture_edge_clamp unavailable\n"
						"Artifacts may occur along texture edges");
					warned = true;
				}
			}

			Texture::bind(this);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexWrapS,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexWrapT,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			Texture::bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if ((*this) && (m_smooth != value))
		{
			if ((m_smooth = value) /*&& !isAvailable()*/)
			{
				// error checking if needed
			}

			Texture::bind(this);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			if (m_mipmapped)
			{
				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMinFilter,
					(m_smooth
						? GL::LinearMipmapLinear
						: GL::LinearMipmapNearest));
			}
			else
			{
				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMinFilter,
					(m_smooth
						? GL::Linear
						: GL::Nearest));
			}

			Texture::bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::setSrgb(bool value)
	{
		if ((*this) && (m_srgb != value))
		{
			if ((m_srgb = value) && !OpenGL::textureSrgbAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::LogWarning(
						"OpenGL extension texture SRGB unavailable");
					warned = true;
				}
			}
			//Texture::bind(this);
			//{}
			//Texture::bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::generateMipmap()
	{
		if ((*this) && (m_mipmapped = OpenGL::framebuffersAvailable()))
		{
			Texture::bind(this);

			OpenGL::generateMipmap(GL::Texture2D);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMinFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMagFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			m_mipmapped = true;

			Texture::bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::invalidateMipmap()
	{
		if ((*this) && m_mipmapped)
		{
			Texture::bind(this);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			m_mipmapped = false;

			Texture::bind(NULL);
		}
		return (*this);
	}


	const Image Texture::copyToImage() const
	{
		// Easy case: empty texture
		if (!(*this))
		{
			return Image();
		}

		// Create an array of pixels
		Image::Pixels pixels(m_size[0] * m_size[1] * 4);

		if ((m_size == m_actualSize))
		{
			// Texture is not padded nor flipped, we can use a direct copy
			Texture::bind(this);
			OpenGL::getTexImage(
				GL::Texture2D,
				0,
				GL::RGBA,
				GL::UnsignedByte,
				&pixels[0]);
			Texture::bind(NULL);
		}

		// Create the image
		Image image;
		image.create(m_size[0], m_size[1], &pixels[0]);
		return image;
	}

	void Texture::bind(const Texture * value)
	{
		OpenGL::bindTexture(
			GL::Texture2D,
			((value && (*value))
				? (Texture::id_type)(*value)
				: NULL));
	}
	
}