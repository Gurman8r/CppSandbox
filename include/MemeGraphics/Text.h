#ifndef _TEXT_H_
#define _TEXT_H_

#include <MemeGraphics/IRenderer.h>
#include <MemeGraphics/Font.h>
#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/Lists.h>

namespace ml
{
	class ML_GRAPHICS_API Text
		: public ITrackable
		, public IRenderer
	{
	public:
		Text();
		Text(const Text & copy);
		~Text();

	public:
		Text & setPosition(const vec2f & value);
		Text & setScale(const vec2f & value);
		Text & setColor(const vec4f & value);
		Text & setFont(const Font * value);
		Text & setFontSize(uint32_t value);
		Text & setText(const String & value);

	public:
		inline const Font *			getFont()		const { return m_font;		}
		inline const uint32_t &		getFontSize()	const { return m_fontSize;	}
		inline const vec2f &		getPosition()	const { return m_position;	}
		inline const vec2f &		getScale()		const { return m_scale;		}
		inline const String &	getText()		const { return m_text;		}
		inline const vec4f &		getColor()		const { return m_color;		}

	public:
		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		mutable bool m_requiresUpdate;
		mutable TextureList	m_textures;
		mutable std::vector<FloatList> m_vertices;

		const Font *	m_font;
		uint32_t		m_fontSize;
		vec2f			m_position;
		vec2f			m_scale;
		String		m_text;
		vec4f			m_color;
	};
}
#endif // !_TEXT_H_
