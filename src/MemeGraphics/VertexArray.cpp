#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray(uint32_t count)
		: m_count(count)
	{
		glCheck(glGenVertexArrays(m_count, &m_id));
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
		glCheck(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const
	{
		glCheck(glBindVertexArray(0));
	}
}