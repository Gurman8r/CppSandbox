#include <MemeGraphics/Text.hpp>

namespace ml
{
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
	
	Text & Text::setString(const String & value)
	{
		if (m_string != value)
		{
			m_requiresUpdate = true;
			m_string = value;
		}
		return (*this);
	}


	void Text::update() const
	{
		if (!m_font)
			return;

		if (m_requiresUpdate)
		{	m_requiresUpdate = false;
			
			m_vertices.resize(m_string.size());
			m_textures.resize(m_string.size());
			
			vec2f drawPos = m_position;
			for (size_t i = 0, imax = m_string.size(); i < imax; i++)
			{
				const Glyph & glyph = (*m_font).getGlyph(m_string[i], m_fontSize);
				
				const FloatRect rect(
					glyph.offset() + drawPos * m_scale,
					glyph.size() * m_scale
				);
				static VertexList verts({
					{ vec3f::Zero, Color::White, vec2f::Zero	},
					{ vec3f::Zero, Color::White, vec2f::Up		},
					{ vec3f::Zero, Color::White, vec2f::One		},
					{ vec3f::Zero, Color::White, vec2f::Zero	},
					{ vec3f::Zero, Color::White, vec2f::One		},
					{ vec3f::Zero, Color::White, vec2f::Right	},
				});
				verts[0].position(rect.left(),  rect.bot(), 0.0f);
				verts[1].position(rect.left(),  rect.top(), 0.0f);
				verts[2].position(rect.right(), rect.top(), 0.0f);
				verts[3].position(rect.left(),  rect.bot(), 0.0f);
				verts[4].position(rect.right(), rect.top(), 0.0f);
				verts[5].position(rect.right(), rect.bot(), 0.0f);
				
				m_vertices[i] = verts;
				m_textures[i] = (&glyph.texture);
				
				switch (m_string[i])
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
		if (!m_font)
			return;

		// Update Geometry/Textures
		update();
		
		// Color
		if (Uniform * u = batch.uniforms->find("u_color")) 
		{ 
			u->data = &getColor(); 
		}

		for (size_t i = 0, imax = m_string.size(); i < imax; i++)
		{
			// Glyph Texture
			if (Uniform * u = batch.uniforms->find("u_texture")) 
			{
				u->data = m_textures[i]; 
			}

			target.draw(&m_vertices[i], batch);
		}
	}
}