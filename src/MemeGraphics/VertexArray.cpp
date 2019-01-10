#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
	{
	}

	VertexArray::VertexArray(uint32_t count)
		: m_count(count)
	{
		m_id = OpenGL::genVertexArrays(m_count);
		bind();
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: m_id(copy.m_id)
		, m_count(copy.m_count)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}


	VertexArray & VertexArray::clean()
	{
		return (*this);
	}


	void VertexArray::bind() const
	{
		OpenGL::bindVertexArray(m_id);
	}

	void VertexArray::unbind() const
	{
		OpenGL::bindVertexArray(0);
	}
}