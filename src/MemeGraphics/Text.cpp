#include <MemeGraphics/Text.h>

namespace ml
{
	Text::Text()
	{
	}
	
	Text::Text(const Text & copy)
	{
	}
	
	Text::~Text()
	{
	}

	
	Text & Text::setPosition(const vec2f & value)
	{
		if (m_position != value)
		{
			m_requiresUpdate = true;
			m_position = value;
		}
		return (*this);
	}
	
	Text & Text::setScale(const vec2f & value)
	{
		if (m_scale != value)
		{
			m_requiresUpdate = true;
			m_scale = value;
		}
		return (*this);
	}
	
	Text & Text::setColor(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}
	
	Text & Text::setFont(const Font * value)
	{
		if (m_font != value)
		{
			m_requiresUpdate = true;
			m_font = value;
		}
		return (*this);
	}
	
	Text & Text::setFontSize(uint32_t value)
	{
		if (m_fontSize != value)
		{
			m_requiresUpdate = true;
			m_fontSize = value;
		}
		return (*this);
	}
	
	Text & Text::setText(const String & value)
	{
		if (m_text != value)
		{
			m_requiresUpdate = true;
			m_text = value;
		}
		return (*this);
	}


	void Text::update() const
	{
		if (m_requiresUpdate)
		{	m_requiresUpdate = false;
			
			m_vertices.resize(m_text.size());
			m_textures.resize(m_text.size());
			
			vec2f drawPos = m_position;
			for (size_t i = 0, imax = m_text.size(); i < imax; i++)
			{
				const Glyph & glyph = (*m_font).getGlyph(m_text[i], m_fontSize);
				
				const FloatRect rect(
					glyph.offset() + drawPos * m_scale,
					glyph.size() * m_scale
				);

				m_vertices[i] = VertexList({
					{{ rect.left(),  rect.bot(), 0.f }, vec2f::Zero	},
					{{ rect.left(),  rect.top(), 0.f }, vec2f::Up	},
					{{ rect.right(), rect.top(), 0.f }, vec2f::One	},
					{{ rect.left(),  rect.bot(), 0.f }, vec2f::Zero	},
					{{ rect.right(), rect.top(), 0.f }, vec2f::One	},
					{{ rect.right(), rect.bot(), 0.f }, vec2f::Right},
				}).contiguous();

				m_textures[i] = (&glyph.texture);
				
				switch (m_text[i])
				{
				case '\n':
					drawPos[0] = m_position[0];
					drawPos[1] -= rect.height() * 2.0f;
					break;
				default:
					drawPos[0] += (glyph.step() * m_scale[0]);
					break;
				}
			}
		}
	}
	
	void Text::draw(RenderTarget & target, RenderBatch batch) const
	{
		update();
		batch.color = &m_color;
		for (size_t i = 0, imax = m_text.size(); i < imax; i++)
		{
			batch.texture = m_textures[i];
			batch.vertices = &m_vertices[i];
			target.draw(batch);
		}
	}
}