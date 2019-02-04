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
		, m_repeated	(false)
		, m_mipmapped	(false)
		, m_colorFmt	(GL::RGBA)
		, m_internalFmt	(GL::RGBA)
	{
	}

	Texture::Texture(const Texture & copy)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(copy.m_smooth)
		, m_repeated	(copy.m_repeated)
		, m_mipmapped	(false)
		, m_colorFmt	(copy.m_colorFmt)
		, m_internalFmt	(copy.m_internalFmt)
	{
		if (copy)
		{
			if (create(copy.width(), copy.height()))
			{
				update(copy);
			}
			else
			{
				Debug::logError("Failed to copy texture, failed to create new texture");
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
		static Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return create(value.width(), value.height()) && update(value);
	}
	
	
	bool Texture::update(const Texture & texture)
	{
		return update(texture.copyToImage());
	}
	
	bool Texture::update(const Image & image)
	{
		return update(image.ptr(), image.width(), image.height(), 0, 0);
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
				x, 
				y, 
				width,
				height,
				m_colorFmt,
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

			OpenGL::texParameter( // TexMinFilter
				GL::Texture2D,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			OpenGL::texParameter( // TexMagFilter
				GL::Texture2D,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			Texture::bind(NULL);

			OpenGL::flush();

			return true;
		}
		return false;
	}


	bool Texture::create(uint32_t width, uint32_t height)
	{
		return create(
			NULL,
			width,
			height,
			m_colorFmt,
			m_internalFmt,
			m_smooth,
			m_repeated
		);
	}

	bool Texture::create(
		const uint8_t * pixels, 
		uint32_t	width, 
		uint32_t	height,
		GL::Format	colorFmt,
		GL::Format	internalFmt, 
		bool		smooth, 
		bool		repeat)
	{
		if (!width || !height)
		{
			return Debug::logError("Failed creating texture, invalid size {0}",
				vec2u(width, height));
		}

		if (!(*this) && (get_ref() = OpenGL::genTextures(1)))
		{
			m_actualSize = vec2u(
				OpenGL::getValidTextureSize(width),
				OpenGL::getValidTextureSize(height));

			const uint32_t maxSize = OpenGL::getMaxTextureSize();
			
			if ((m_actualSize[0] > maxSize) || (m_actualSize[1] > maxSize))
			{
				return Debug::logError(
					"Failed creating texture: "
					"Internal size is too high {0} "
					"Maximum is {1}",
					m_actualSize,
					vec2u(maxSize));
			}

			m_size		= { width, height };
			m_smooth	= smooth;
			m_repeated	= repeat;
			m_colorFmt	= colorFmt;
			m_internalFmt	= internalFmt;

			if (m_mipmapped && !OpenGL::framebuffersAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
					warned = true;
				}
			}

			if (!m_repeated && !OpenGL::edgeClampAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Edge Clamp Unavailable");
					warned = true;
				}
			}

			Texture::bind(this);

			OpenGL::texImage2D(
				GL::Texture2D,
				0,
				m_colorFmt,
				m_size[0],
				m_size[1],
				0,
				m_internalFmt,
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

			OpenGL::texParameter( // TexMinFilter
				GL::Texture2D,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			OpenGL::texParameter( // TexMagFilter
				GL::Texture2D,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			Texture::bind(NULL);

			OpenGL::flush();

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
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);
		std::swap(m_colorFmt,	other.m_colorFmt);
		std::swap(m_internalFmt,	other.m_internalFmt);

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
					Debug::logWarning(
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

			if (m_mipmapped)
			{
				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMinFilter,
					(m_smooth
						? GL::LinearMipmapLinear
						: GL::LinearMipmapNearest));

				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMagFilter,
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

				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMagFilter,
					(m_smooth
						? GL::Linear
						: GL::Nearest));
			}

			Texture::bind(NULL);
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

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMagFilter,
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
		if ((*this))
		{
			Image::Pixels pixels(width() * height() * 4);

			if ((m_size == m_actualSize))
			{
				Texture::bind(this);

				OpenGL::getTexImage(
					GL::Texture2D,
					0,
					m_colorFmt,
					GL::UnsignedByte,
					&pixels[0]);

				Texture::bind(NULL);
			}

			return Image().create(width(), height(), &pixels[0]);
		}
		return Image();
	}

	void Texture::bind(const Texture * value)
	{
		OpenGL::bindTexture(
			GL::Texture2D,
			((value && (*value))
				? (Texture::value_type)(*value)
				: NULL));
	}
	
}