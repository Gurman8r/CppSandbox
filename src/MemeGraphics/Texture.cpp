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
		: Texture(target, true, false)
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

	Texture::Texture(GL::Target target, GL::Format format, bool smooth, bool repeated)
		: Texture(target, format, smooth, repeated, false)
	{
	}

	Texture::Texture(GL::Target target, GL::Format format, bool smooth, bool repeated, bool mipmapped)
		: Texture(target, format, format, smooth, repeated, mipmapped)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated)
		: Texture(target, intFormat, colFormat, smooth, repeated, false)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped)
		: Texture(target, intFormat, colFormat, smooth, repeated, mipmapped, 0, 0, GL::UnsignedByte)
	{
	}

	Texture::Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, int32_t border, GL::Type type)
		: IHandle		(NULL)
		, m_size		(vec2u::Zero)
		, m_realSize	(vec2u::Zero)
		, m_target		(target)
		, m_intFormat	(intFormat)
		, m_colFormat	(colFormat)
		, m_smooth		(smooth)
		, m_repeated	(repeated)
		, m_mipmapped	(mipmapped)
		, m_level		(level)
		, m_border		(border)
		, m_type		(type)
	{
	}

	Texture::Texture(const Texture & copy)
		: Texture(copy.m_target, copy.m_intFormat, copy.m_colFormat, copy.m_smooth, copy.m_repeated, copy.m_mipmapped, copy.m_level, copy.m_border, copy.m_type)
	{
		create(copy);
	}

	Texture::~Texture()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Texture::dispose()
	{
		bind(NULL);
		if ((*this))
		{
			ML_GL.deleteTextures(1, (*this));
			get_reference() = NULL;
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

			ML_GL.texSubImage2D(
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

			ML_GL.flush();

			setRepeated(m_repeated);

			setSmooth(m_smooth);

			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Texture::create(const Texture & copy)
	{
		if (copy)
		{
			if (create(copy.size()))
			{
				return update(copy);
			}
			else
			{
				return Debug::logError("Failed to copy texture, failed to create new texture");
			}
		}
		return false;
	}

	bool Texture::create(const vec2u & size)
	{
		return create(NULL, size);
	}

	bool Texture::create(const uint8_t * pixels, const vec2u & size)
	{
		if (!size[0] || !size[1])
		{
			return Debug::logError("Failed creating texture, invalid getSize {0}", size);
		}

		if (set_handle(ML_GL.genTextures(1)))
		{
			m_size = size;

			m_realSize = vec2u(
				ML_GL.getValidTextureSize(m_size[0]),
				ML_GL.getValidTextureSize(m_size[1]));

			static const uint32_t maxSize = ML_GL.getMaxTextureSize();
			if ((m_realSize[0] > maxSize) || (m_realSize[1] > maxSize))
			{
				return Debug::logError(
					"Failed creating texture: "
					"Internal getSize is too high {0} "
					"Maximum is {1}",
					m_realSize,
					vec2u(maxSize));
			}

			bind(this);

			ML_GL.texImage2D(
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

			ML_GL.flush();

			setRepeated(m_repeated);

			setSmooth(m_smooth);

			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::setMipmapped(bool value)
	{
		if ((*this))
		{
			if ((m_mipmapped = value) && !ML_GL.framebuffersAvailable())
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

			ML_GL.generateMipmap(m_target);

			ML_GL.texParameter(
				m_target,
				GL::TexMinFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			ML_GL.texParameter(
				m_target,
				GL::TexMagFilter,
				((m_smooth
					? GL::LinearMipmapLinear
					: GL::NearestMipmapNearest)));

			bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setRepeated(bool value)
	{
		if ((*this))
		{
			if ((m_repeated = value) && !ML_GL.edgeClampAvailable())
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

			ML_GL.texParameter(
				m_target,
				GL::TexWrapS,
				(m_repeated
					? GL::Repeat
					: (ML_GL.edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			ML_GL.texParameter(
				m_target,
				GL::TexWrapT,
				(m_repeated
					? GL::Repeat
					: (ML_GL.edgeClampAvailable()
						? GL::ClampToEdge
						: GL::Clamp)));

			bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if ((*this))
		{
			m_smooth = value;

			bind(this);

			ML_GL.texParameter(
				m_target,
				GL::TexMinFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			ML_GL.texParameter(
				m_target,
				GL::TexMagFilter,
				(m_smooth
					? GL::Linear
					: GL::Nearest));

			bind(NULL);

			ML_GL.flush();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Texture & Texture::swap(Texture & other)
	{
		std::swap(get_reference(),	other.get_reference());
		std::swap(m_target,		other.m_target);
		std::swap(m_level,		other.m_level);
		std::swap(m_size,		other.m_size);
		std::swap(m_realSize,	other.m_realSize);
		std::swap(m_border,		other.m_border);
		std::swap(m_intFormat,	other.m_intFormat);
		std::swap(m_colFormat,	other.m_colFormat);
		std::swap(m_type,		other.m_type);
		std::swap(m_smooth,		other.m_smooth);
		std::swap(m_repeated,	other.m_repeated);
		std::swap(m_mipmapped,	other.m_mipmapped);

		return other;
	}
	
	Texture & Texture::operator=(const Texture & value)
	{
		static Texture temp;
		return swap(temp);
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

				ML_GL.getTexImage(
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

	void Texture::bind(const Texture * value)
	{
		if (value)
		{
			ML_GL.bindTexture(value->target(), (*value));
		}
		else
		{
			ML_GL.bindTexture(GL::Texture2D, NULL);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Texture::serialize(std::ostream & out) const
	{
		out << GetTypeName();
	}

	void Texture::deserialize(std::istream & in)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}