#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <MemeCore/Rect.hpp>
#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Image.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API Texture final
		: public ITrackable
		, public IReadable
		, public IHandle<uint32_t>
	{
	public:
		Texture();
		Texture(GL::Target target);
		Texture(bool smooth, bool repeated);
		Texture(GL::Target target, bool smooth, bool repeated);
		Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated);
		Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped);
		Texture(GL::Target target, GL::Format intFormat, GL::Format colFormat, bool smooth, bool repeated, bool mipmapped, int32_t level, int32_t border, GL::Type type);
		Texture(const Texture & copy);
		~Texture();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, uint32_t target);
		bool loadFromImage(const Image & value);
		bool loadFromImage(const Image & value, uint32_t target);

	public:
		bool update(const Texture & texture);
		bool update(const Image & image);
		bool update(const Image & image, const vec2u & pos, const vec2u & size);
		bool update(const uint8_t * pixels, const vec2u & pos, const vec2u & size);

	public:
		bool create(const vec2u & size);
		bool create(const uint8_t * pixels, const vec2u & size);

	public:
		Texture & setMipmapped(bool value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		
		Texture & swap(Texture & value);
		Texture & operator=(const Texture & value);

		const Image copyToImage() const;
		static void	bind(const Texture * value);

	public:
		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const GL::Target	target()	const { return m_target;	}
		inline const int32_t	level()		const { return m_level;		}
		inline const vec2u &	size()		const { return m_size;		}
		inline const vec2u &	realSize()	const { return m_realSize;	}
		inline const int32_t	border()	const { return m_border;	}
		inline const GL::Format intFormat()	const { return m_intFormat;	}
		inline const GL::Format colFormat()	const { return m_colFormat; }
		inline const GL::Type	type()		const { return m_type;		}
		inline const bool		smooth()	const { return m_smooth;	}
		inline const bool		repeated()	const { return m_repeated;	}
		inline const bool		mipmapped()	const { return m_mipmapped; }
		inline const uint32_t	width()		const { return size()[0];	}
		inline const uint32_t	height()	const { return size()[1];	}

	private:
		GL::Target	m_target;
		int32_t		m_level;
		vec2u		m_size;
		vec2u		m_realSize;
		int32_t		m_border;
		GL::Format	m_intFormat;
		GL::Format	m_colFormat;
		GL::Type	m_type;
		bool		m_smooth;
		bool		m_repeated;
		bool		m_mipmapped;

	};
}

#endif // !_TEXTURE_HPP_
