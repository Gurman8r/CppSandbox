#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Texture::Texture()
		: IHandle		(NULL)
		, m_type		(GL::Texture2D)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(false)
		, m_repeated	(false)
		, m_mipmapped	(false)
		, m_colFormat	(GL::RGBA)
		, m_intFormat	(GL::RGBA)
	{
	}

	Texture::Texture(const Texture & copy)
		: IHandle		(NULL)
		, m_type		(copy.m_type)
		, m_size		(vec2u::Zero)
		, m_actualSize	(vec2u::Zero)
		, m_smooth		(copy.m_smooth)
		, m_repeated	(copy.m_repeated)
		, m_mipmapped	(false)
		, m_colFormat	(copy.m_colFormat)
		, m_intFormat	(copy.m_intFormat)
	{
		if (copy)
		{
			if (create(copy.size()))
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

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::cleanup()
	{
		bind(NULL);
		if ((*this))
		{
			OpenGL::deleteTextures(1, (*this));
			get_ref() = NULL;
			return true;
		}
		return false;
	}

	bool Texture::loadFromFile(const String & filename)
	{
		return loadFromFile(filename, m_type);
	}

	bool Texture::loadFromFile(const String & filename, uint32_t type)
	{
		static Image image;
		return image.loadFromFile(filename) && loadFromImage(image, type);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return loadFromImage(value, m_type);
	}

	bool Texture::loadFromImage(const Image & value, uint32_t type)
	{
		return create(value.size()) && update(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Texture::serialize(std::ostream & out) const
	{
	}

	void Texture::deserialize(std::istream & in)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::update(const Texture & texture)
	{
		return update(texture.copyToImage());
	}
	
	bool Texture::update(const Image & image)
	{
		return update(image.ptr(), image.size(), vec2u::Zero);
	}

	bool Texture::update(const uint8_t * pixels, const vec2u & size, const vec2u & pos)
	{
		if ((*this) && pixels)
		{
			bind(this);
			OpenGL::texSubImage2D(
				m_type,
				0,
				pos[0],
				pos[1],
				size[0],
				size[1],
				m_colFormat,
				GL::UnsignedByte,
				pixels);
			bind(NULL);

			setRepeated(m_repeated);

			setSmooth(m_smooth);

			OpenGL::flush();

			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const vec2u & size)
	{
		return create(size, m_type);
	}

	bool Texture::create(const vec2u & size, uint32_t type)
	{
		return create(
			NULL,
			size,
			m_colFormat,
			m_intFormat,
			m_smooth,
			m_repeated,
			type
		);
	}

	bool Texture::create(
		const uint8_t * pixels, 
		const vec2u &	size,
		GL::Format		colFormat,
		GL::Format		intFormat, 
		bool			smooth, 
		bool			repeat,
		uint32_t		type)
	{
		if (!size[0] || !size[1])
		{
			return Debug::logError("Failed creating texture, invalid size {0}", size);
		}

		if (!(*this) && (get_ref() = OpenGL::genTextures(1)))
		{
			m_actualSize = vec2u(
				OpenGL::getValidTextureSize(size[0]),
				OpenGL::getValidTextureSize(size[1]));

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

			m_type		= type;
			m_size		= size;
			m_colFormat	= colFormat;
			m_intFormat	= intFormat;

			bind(this);
			OpenGL::texImage2D(
				m_type,
				0,
				m_colFormat,
				m_size[0],
				m_size[1],
				0,
				m_intFormat,
				GL::UnsignedByte,
				pixels);
			bind(NULL);

			setRepeated(repeat);

			setSmooth(smooth);

			OpenGL::flush();

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_ref(),	other.get_ref());
		std::swap(m_size,		other.m_size);
		std::swap(m_actualSize,	other.m_actualSize);
		std::swap(m_smooth,		other.m_smooth);
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);
		std::swap(m_colFormat,	other.m_colFormat);
		std::swap(m_intFormat,	other.m_intFormat);

		return other;
	}

	Texture & Texture::setRepeated(bool value)
	{
		if ((*this))
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

			bind(this);

			OpenGL::texParameter(
				m_type,
				GL::TexWrapS,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			OpenGL::texParameter(
				m_type,
				GL::TexWrapT,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if ((*this))
		{
			m_smooth = value;

			bind(this);

			OpenGL::texParameter(
				m_type,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			OpenGL::texParameter(
				m_type,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::generateMipmap()
	{
		if (!OpenGL::framebuffersAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
				warned = true;
			}
		}

		if ((*this) && (m_mipmapped = OpenGL::framebuffersAvailable()))
		{
			bind(this);

			OpenGL::generateMipmap(m_type);

			OpenGL::texParameter(
				m_type,
				GL::TexMinFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			OpenGL::texParameter(
				m_type,
				GL::TexMagFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			m_mipmapped = true;

			bind(NULL);
		}
		return (*this);
	}

	Texture & Texture::invalidateMipmap()
	{
		if ((*this) && m_mipmapped)
		{
			bind(this);

			OpenGL::texParameter(
				m_type,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			OpenGL::texParameter(
				m_type,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			m_mipmapped = false;

			bind(NULL);
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Image Texture::copyToImage() const
	{
		if ((*this))
		{
			Image::Pixels pixels(width() * height() * 4);

			if ((m_size == m_actualSize))
			{
				bind(this);

				OpenGL::getTexImage(
					m_type,
					0,
					m_colFormat,
					GL::UnsignedByte,
					&pixels[0]);

				bind(NULL);
			}

			return Image().create(width(), height(), &pixels[0]);
		}
		return Image();
	}

	void Texture::bind(const Texture * value)
	{
		if (value)
		{
			OpenGL::bindTexture(value->type(), (uint32_t)(*value));
		}
		else
		{
			OpenGL::bindTexture(GL::Texture2D, NULL);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}