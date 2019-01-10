#include <MemeGraphics/Text.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/RenderTarget.h>
#include <MemeCore/Rect.h>

namespace ml
{
	Text::Text()
		: m_shader(NULL)
		, m_font(NULL)
		, m_fontSize(0)
		, m_color(Color::White)
		, m_string()
	{
	}

	Text::Text(Shader * shader, Font * font)
		: m_shader(shader)
		, m_font(font)
		, m_fontSize(0)
		, m_color(Color::White)
		, m_string()
	{
	}

	Text::Text(const Text & copy)
		: m_shader(copy.m_shader)
		, m_font(copy.m_font)
		, m_fontSize(copy.m_fontSize)
		, m_color(copy.m_color)
		, m_string(copy.m_string)
	{
	}

	Text::~Text()
	{
	}


	const vec4f & Text::color() const
	{
		return m_color;
	}

	const Font * Text::font() const
	{
		return m_font;
	}

	const uint32_t & Text::fontSize() const
	{
		return m_fontSize;
	}

	const Shader * Text::shader() const
	{
		return m_shader;
	}

	const std::string & Text::string() const
	{
		return m_string;
	}


	Text & Text::color(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}

	Text & Text::font(Font * value)
	{
		m_font = value;
		return (*this);
	}

	Text & Text::fontSize(uint32_t value)
	{
		m_fontSize = value;
		return (*this);
	}

	Text & Text::shader(Shader * value)
	{
		m_shader = value;
		return (*this);
	}

	Text & Text::string(const std::string & value)
	{
		m_string = value;
		return (*this);
	}


	bool Text::isValid() const
	{
		return 
			shader() &&
			font();
	}

	Text::operator bool() const
	{
		return isValid();
	}


	void Text::draw(RenderTarget & target, RenderState state) const
	{
		if (isValid())
		{
			target.disable(GL::Flag::CullFace);

			vec2f pos = transform().position();
			vec2f scale = transform().localScale();

			for (const char & c : string())
			{
				const Glyph & g = m_font->getGlyph(c, m_fontSize);

				float l = pos[0] + g.bounds.left() * scale[0];
				float t = pos[1] - (g.bounds.height() - g.bounds.top()) * scale[1];
				float r = l + g.bounds.width() * scale[0];
				float b = t + g.bounds.height() * scale[1];

				switch (c)
				{
				case '\n':
					pos[1] -= (b - t) * 2.0f;
					pos[0] = transform().position()[0];
					continue;
				}

				const VertexList vertices({
					Vertex(vec3f(l,	b,  0), Color::White, vec2f::Zero),
					Vertex(vec3f(l,	t,	0), Color::White, vec2f::Up),
					Vertex(vec3f(r,	t,	0), Color::White, vec2f::One),
					Vertex(vec3f(l,	b,	0), Color::White, vec2f::Zero),
					Vertex(vec3f(r,	t,	0), Color::White, vec2f::One),
					Vertex(vec3f(r,	b,  0), Color::White, vec2f::Right),
				});

				state.textureHandle = g.texture;
				state.transform = Transform(pos, scale, 0);
				state.shader = shader();
				state.color = color();

				//target->drawDynamic(Enum::Primitive::Triangles, vertices, state);

				pos[0] += (g.advance >> vertices.size()) * scale[0];
			}

			target.enable(GL::Flag::CullFace);
		}
	}
}