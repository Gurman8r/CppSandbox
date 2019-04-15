#ifndef _ML_TEXT_HPP_
#define _ML_TEXT_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Font.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/Shapes.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Text
		: public ITrackable
		, public IDrawable
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
		Text & setString(const String & value);

	public:
		inline const Font *		getFont()		const { return m_font;		}
		inline const uint32_t	getFontSize()	const { return m_fontSize;	}
		inline const vec2f &	getPosition()	const { return m_position;	}
		inline const vec2f &	getScale()		const { return m_scale;		}
		inline const String &	getString()		const { return m_string;	}
		inline const vec4f &	getColor()		const { return m_color;		}

	public:
		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const Font *m_font;
		uint32_t	m_fontSize;
		vec2f		m_position;
		vec2f		m_scale;
		String		m_string;
		vec4f		m_color;

		mutable bool			m_requiresUpdate;
		mutable TextureList		m_textures;
		mutable List<RectQuad>	m_vertices;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_TEXT_HPP_