#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Texture::Texture()
		: Texture(GL::Texture2D)
	{
	}

	Texture::Texture(GL::Target target)
		: Texture(target, false, false)
	{
	}

	Texture::Texture(bool smooth, bool repeated)
		: Texture(GL::Texture2D, smooth, repeated)
	{
	}

	Texture::Texture(GL::Target target, bool smooth, bool repeated)
		: Texture(target, GL::RGBA, smooth, repeated)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, bool smooth, bool repeated)
		: Texture(target, intFormat, smooth, repeated, false)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, bool smooth, bool repeated, bool mipmapped)
		: Texture(target, intFormat, intFormat, smooth, repeated, mipmapped)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated)
		: Texture(target, intFormat, colFormat, smooth, repeated, false)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped)
		: Texture(target, intFormat, intFormat, smooth, repeated, mipmapped, 0, 0, GL::UnsignedByte)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, int32_t border, GL::Type type)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_realSize	(vec2u::Zero)
		, m_target		(target)
		, m_smooth		(smooth)
		, m_repeated	(repeated)
		, m_mipmapped	(mipmapped)
		, m_intFormat		(intFormat)
		, m_colFormat	(colFormat)
		, m_level		(level)
		, m_border		(border)
		, m_type		(type)
	{
	}

	Texture::Texture(const Texture & copy)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_realSize	(vec2u::Zero)
		, m_target		(copy.m_target)
		, m_smooth		(copy.m_smooth)
		, m_repeated	(copy.m_repeated)
		, m_mipmapped	(copy.m_mipmapped)
		, m_intFormat		(copy.m_intFormat)
		, m_colFormat	(copy.m_colFormat)
		, m_level		(copy.m_level)
		, m_border		(copy.m_border)
		, m_type		(copy.m_type)
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
		return loadFromFile(filename, m_target);
	}

	bool Texture::loadFromFile(const String & filename, uint32_t target)
	{
		static Image image;
		return image.loadFromFile(filename) && loadFromImage(image, target);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return loadFromImage(value, m_target);
	}

	bool Texture::loadFromImage(const Image & value, uint32_t target)
	{
		return create(value.size()) && update(value);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Texture::serialize(std::ostream & out) const
	{
		out << get_type().name();
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
		return update(image, vec2u::Zero, image.size());
	}

	bool Texture::update(const Image & image, const vec2u & pos, const vec2u & size)
	{
		return update(image.ptr(), pos, size);
	}

	bool Texture::update(const uint8_t * pixels, const vec2u & pos, const vec2u & size)
	{
		if ((*this) && pixels)
		{
			bind(this);

			OpenGL::texSubImage2D(
				m_target,
				m_level,
				pos [0],
				pos [1],
				size[0],
				size[1],
				m_intFormat,
				m_type,
				pixels);

			bind(NULL);

			OpenGL::flush();

			setRepeated(m_repeated);

			setSmooth(m_smooth);

			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const vec2u & size)
	{
		return create(size, m_target);
	}

	bool Texture::create(const vec2u & size, GL::Target target)
	{
		return create(
			target,
			NULL,
			size,
			m_intFormat,
			m_colFormat,
			m_smooth,
			m_repeated
		);
	}

	bool Texture::create(const uint8_t * pixels, const vec2u & size)
	{
		return create(
			m_target,
			pixels,
			size,
			m_intFormat,
			m_colFormat,
			m_smooth,
			m_repeated
		);
	}

	bool Texture::create(GL::Target target, const uint8_t * pixels, const vec2u & size, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated)
	{
		if (!size[0] || !size[1])
		{
			return Debug::logError("Failed creating texture, invalid size {0}", size);
		}

		if (!(*this) && (get_ref() = OpenGL::genTextures(1)))
		{
			m_realSize = vec2u(
				OpenGL::getValidTextureSize(size[0]),
				OpenGL::getValidTextureSize(size[1]));
			static const uint32_t maxSize = OpenGL::getMaxTextureSize();
			if ((m_realSize[0] > maxSize) || (m_realSize[1] > maxSize))
			{
				return Debug::logError(
					"Failed creating texture: "
					"Internal size is too high {0} "
					"Maximum is {1}",
					m_realSize,
					vec2u(maxSize));
			}

			m_target	= target;
			m_size		= size;
			m_intFormat	= intFormat;
			m_colFormat	= colFormat;

			bind(this);

			OpenGL::texImage2D(
				m_target,
				m_level,
				m_intFormat,
				m_size[0],
				m_size[1],
				m_border,
				m_colFormat,
				m_type,
				pixels);

			bind(NULL);

			OpenGL::flush();

			setRepeated(repeated);

			setSmooth(smooth);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::setMipmapped(bool value)
	{
		if ((*this))
		{
			if ((m_mipmapped = value) && !OpenGL::framebuffersAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
					warned = true;
				}
				m_mipmapped = false;
			}

			if (!m_mipmapped)
			{
				return setSmooth(m_smooth);
			}

			bind(this);

			OpenGL::generateMipmap(m_target);

			OpenGL::texParameter(
				m_target,
				GL::TexMinFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			OpenGL::texParameter(
				m_target,
				GL::TexMagFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			bind(NULL);

			OpenGL::flush();
		}
		return (*this);
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
				m_target,
				GL::TexWrapS,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			OpenGL::texParameter(
				m_target,
				GL::TexWrapT,
				(m_repeated
					? GL::Repeat
					: (OpenGL::edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			bind(NULL);

			OpenGL::flush();
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
				m_target,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			OpenGL::texParameter(
				m_target,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			bind(NULL);

			OpenGL::flush();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const Image Texture::copyToImage() const
	{
		if ((*this))
		{
			Image::Pixels pixels(width() * height() * 4);

			if ((m_size == m_realSize))
			{
				bind(this);

				OpenGL::getTexImage(
					m_target,
					0,
					m_intFormat,
					GL::UnsignedByte,
					&pixels[0]);

				bind(NULL);
			}

			return Image().create(width(), height(), &pixels[0]);
		}
		return Image();
	}

	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_ref(),	other.get_ref());
		std::swap(m_size,		other.m_size);
		std::swap(m_realSize,	other.m_realSize);
		std::swap(m_smooth,		other.m_smooth);
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);
		std::swap(m_intFormat,	other.m_intFormat);
		std::swap(m_colFormat,	other.m_colFormat);

		return other;
	}
	
	Texture & Texture::operator=(const Texture & value)
	{
		static Texture temp;
		return swap(temp);
	}

	void Texture::bind(const Texture * value)
	{
		if (value)
		{
			OpenGL::bindTexture(value->target(), (uint32_t)(*value));
		}
		else
		{
			OpenGL::bindTexture(GL::Texture2D, NULL);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}