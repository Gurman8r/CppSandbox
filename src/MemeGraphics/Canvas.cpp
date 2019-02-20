#include <MemeGraphics/Canvas.hpp>
#include <MemeGraphics/BufferLayout.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/Text.hpp>
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
			m_vao.create(ml::GL::Triangles).bind();
			
			m_vbo.create(ml::GL::DynamicDraw).bind();

			m_vbo.bufferData(NULL, (ml::Glyph::VertexCount * ml::Vertex::Size));
			
			ml::BufferLayout::Default.bind();
			
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