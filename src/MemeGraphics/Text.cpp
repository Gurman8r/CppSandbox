#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/ShaderAPI.hpp>
#include <MemeGraphics/Uni.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Text::Text()
		: m_font	(NULL)
		, m_fontSize(0)
		, m_position(0)
		, m_scale	(1)
		, m_string	(String())
		, m_color	(Color::White)

	{
	}
	
	Text::Text(const Text & copy)
		: m_font	(copy.m_font)
		, m_fontSize(copy.m_fontSize)
		, m_position(copy.m_position)
		, m_scale	(copy.m_scale)
		, m_string	(copy.m_string)
		, m_color	(copy.m_color)
	{
	}
	
	Text::~Text()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Text & Text::setPosition(const vec2 & value)
	{
		if (m_position != value)
		{
			m_changed = true;
			m_position = value;
		}
		return (*this);
	}
	
	Text & Text::setScale(const vec2 & value)
	{
		if (m_scale != value)
		{
			m_changed = true;
			m_scale = value;
		}
		return (*this);
	}
	
	Text & Text::setColor(const vec4 & value)
	{
		m_color = value;
		return (*this);
	}
	
	Text & Text::setFont(const Font * value)
	{
		if (m_font != value)
		{
			m_changed = true;
			m_font = value;
		}
		return (*this);
	}
	
	Text & Text::setFontSize(uint32_t value)
	{
		if (m_fontSize != value)
		{
			m_changed = true;
			m_fontSize = value;
		}
		return (*this);
	}
	
	Text & Text::setString(const String & value)
	{
		if (m_string != value)
		{
			m_changed = true;
			m_string = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Text::update() const
	{
		if (m_font && m_changed)
		{	
			m_changed = false;
			
			m_vertices.resize(m_string.size());
			m_textures.resize(m_string.size());
			
			vec2 pos = getPosition();

			for (size_t i = 0, imax = m_string.size(); i < imax; i++)
			{
				const Glyph & glyph = m_font->getGlyph(m_string[i], m_fontSize);
				
				const FloatRect rect(
					glyph.offset() + pos * m_scale,
					glyph.size() * m_scale
				);

				m_vertices[i] = Shapes::RectQuad::genGlyphQuad(rect);
				m_textures[i] = (&glyph.texture);
				
				switch (m_string[i])
				{
				case '\n':
					pos[0] = m_position[0];
					pos[1] -= rect.height() * 2.0f;
					break;
				default:
					pos[0] += (glyph.step() * m_scale[0]);
					break;
				}
			}
		}
	}
	
	void Text::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_font)
		{
			update();

			if (uni_col4 * u = batch.mat->find_uni<uni_col4>(ML_FRAG_MAIN_COL))
			{
				u->data = m_color;
			}

			for (size_t i = 0, imax = m_string.size(); i < imax; i++)
			{
				if (uni_tex_cp * u = batch.mat->find_uni<uni_tex_cp>(ML_FRAG_MAIN_TEX))
				{
					u->data = m_textures[i];
				}

				target.draw(m_vertices[i].data(), Shapes::RectQuad::Size, batch);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}