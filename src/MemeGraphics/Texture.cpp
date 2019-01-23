#include <MemeGraphics/Texture.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <cassert>
#include <algorithm>

namespace ml
{
	Texture::Texture()
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(false)
		, m_sRGB		(false)
		, m_repeated	(false)
		, m_mipmapped	(false)
	{
	}

	Texture::Texture(const Texture & copy)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(copy.m_smooth)
		, m_sRGB		(copy.m_sRGB)
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

	bool Texture::loadFromFile(const std::string & filename)
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
		int32_t width = static_cast<int32_t>(image.size()[0]);
		int32_t height = static_cast<int32_t>(image.size()[1]);

		// Load the entire image if the source area
		// is either empty or contains the whole image
		if ((area.width() == 0 || area.height() == 0) || (
			area.left() <= 0 && 
			area.top() <= 0 && 
			area.width() >= width && 
			area.height() >= height))
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
			if (rect.left() < 0)		rect.left(0);
			if (rect.top() < 0)			rect.top(0);
			if (rect.right() > width)	rect.right(width);
			if (rect.bot() > height)	rect.bot(height);

			// Create the texture and upload the pixels
			if (create(rect.width(), rect.height()))
			{
				// Copy the pixels to the texture, row by row
				const uint8_t * pixels = 
					image.ptr() + 4 * (rect.left() + (width * rect.top()));

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

					pixels += 4 * width;
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
	
	bool Texture::update(const uint8_t * pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
	{
		assert(x + width <= m_size[0]);
		assert(y + height <= m_size[1]);

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
			return Debug::LogError("Failed creating texture, invalid size ( {0} x {1} )",
				m_size[0], 
				m_size[1]);
		}

		vec2u actualSize(
			OpenGL::getValidTextureSize(width),
			OpenGL::getValidTextureSize(height));
		uint32_t maxSize = OpenGL::getMaxTextureSize();
		if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
		{
			return Debug::LogError(
				"Failed creating texture: "
				"Internal size is too high ( {0} x {1} ) "
				"Maximum is ( {2} x {2} )", 
				actualSize[0],
				actualSize[1], 
				maxSize);
		}

		m_actualSize = actualSize;
		m_size = vec2u(width, height);

		if (!(*this) && (handle() = OpenGL::genTextures(1)))
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

		if (m_sRGB && !OpenGL::textureSrgbAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::LogWarning(
					"OpenGL ES extension EXT_sRGB unavailable\n"
					"Automatic sRGB to linear conversion disabled");
				warned = true;
			}
			m_sRGB = false;
		}

		m_mipmapped = false;

		// Initialize the texture
		Texture::bind(this);
		
		OpenGL::texImage2D(
			GL::Texture2D,
			0,
			(m_sRGB
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

	bool Texture::create(uint32_t width, uint32_t height, const vec4f & color)
	{
		static Image image;
		image.create(width, height, color);
		return loadFromImage(image);
	}

	bool Texture::create(
		uint32_t width, 
		uint32_t height,
		const uint8_t * pixels, 
		GL::Format colFmt,
		GL::Format intFmt, 
		bool smooth, 
		bool repeat,
		bool mipmapped)
	{
		if (!(*this) && (handle() = OpenGL::genTextures(1)))
		{
			if (!width || !height)
			{
				return Debug::LogError("Failed creating texture, invalid size ( {0} x {1} )",
						width,
						height);
			}

			vec2u actualSize(
				OpenGL::getValidTextureSize(width),
				OpenGL::getValidTextureSize(height));
			uint32_t maxSize = OpenGL::getMaxTextureSize();
			if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
			{
				return Debug::LogError(
					"Failed creating texture: "
					"Internal size is too high ( {0} x {1} ) "
					"Maximum is ( {2} x {2} )",
					actualSize[0],
					actualSize[1],
					maxSize);
			}

			m_size			= { width, height };
			m_actualSize	= actualSize;
			m_smooth		= smooth;
			m_sRGB			= false;
			m_repeated	= repeat;
			m_mipmapped		= false;
		
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
		std::swap(handle(),		other.handle());
		std::swap(m_size,		other.m_size);
		std::swap(m_actualSize,	other.m_actualSize);
		std::swap(m_smooth,	other.m_smooth);
		std::swap(m_sRGB,		other.m_sRGB);
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
		if ((*this) && (m_sRGB != value))
		{
			if ((m_sRGB = value) && !OpenGL::textureSrgbAvailable())
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
			(m_smooth
				? GL::LinearMipmapLinear
				: GL::NearestMipmapNearest));

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
		std::vector<uint8_t> pixels(m_size[0] * m_size[1] * 4);

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
				? value->id() 
				: NULL));
	}
	
}