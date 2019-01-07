#include <MemeGraphics/Sprite.h>
#include <MemeGraphics/Shapes.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/RenderTarget.h>

namespace ml
{
	Sprite::Sprite()
		: m_shader(NULL)
		, m_texture(NULL)
		, m_color(Color::White)
		, m_mesh(Shapes::Quad::Mesh)
	{
	}

	Sprite::Sprite(Shader * shader, Texture * texture)
		: m_shader(shader)
		, m_texture(texture)
		, m_color(Color::White)
		, m_mesh(Shapes::Quad::Mesh)
	{
	}

	Sprite::Sprite(const Sprite & copy)
		: m_shader(copy.m_shader)
		, m_texture(copy.m_texture)
		, m_color(copy.m_color)
		, m_mesh(copy.m_mesh)
	{
	}

	Sprite::~Sprite()
	{
	}


	const vec4f & Sprite::color() const
	{
		return m_color;
	}

	const Mesh * Sprite::mesh() const
	{
		return &m_mesh;
	}

	const Shader * Sprite::shader() const
	{
		return m_shader;
	}

	const Texture * Sprite::texture() const
	{
		return m_texture;
	}


	Sprite & Sprite::color(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}

	Sprite & Sprite::shader(Shader * value)
	{
		m_shader = value;
		return (*this);
	}

	Sprite & Sprite::texture(Texture * value)
	{
		m_texture = value;
		return (*this);
	}


	bool Sprite::isValid() const
	{
		return 
			shader() &&
			texture();
	}

	Sprite::operator bool() const
	{
		return isValid();
	}


	void Sprite::draw(RenderTarget & target, RenderState state) const
	{
		if (isValid())
		{
			target.disableFlag(Enum::Flag::DepthTest);
			target.disableFlag(Enum::Flag::CullFace);

			state.shader = shader();
			state.texture = texture();
			state.color = color();
			state.mesh = mesh();

			target.draw(state);

			target.enableFlag(Enum::Flag::DepthTest);
			target.enableFlag(Enum::Flag::CullFace);
		}
	}
}