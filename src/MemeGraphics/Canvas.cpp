#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/Shapes.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Canvas::Canvas()
		: m_vao		()
		, m_vbo		()
		, m_shader	(NULL)
		, m_uniforms(NULL)
	{
	}

	Canvas::~Canvas()
	{
	}
	bool Canvas::create()
	{
		if (!m_vao && !m_vbo)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::DynamicDraw).bind();
			m_vbo.bufferData(NULL, RectQuad::Size);
			BufferLayout::Default.bind();
			m_vbo.unbind();
			m_vao.unbind();
		}
		return false;
	}
	
	bool Canvas::setShader(const Shader * value)
	{
		return (m_shader = value);
	}
	
	bool Canvas::setUniforms(const UniformSet * value)
	{
		return (m_uniforms = value);
	}
}