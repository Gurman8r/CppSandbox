#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexBuffer::VertexBuffer(Usage usage, const void * data, uint32_t size)
		: m_usage(usage)
	{
		glCheck(glGenBuffers(1, &m_id));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		glCheck(glBufferData(GL_ARRAY_BUFFER, (size * sizeof(float)), data, static_cast<GLenum>(m_usage)));
	}

	VertexBuffer::~VertexBuffer()
	{
		clean();
	}

	VertexBuffer & VertexBuffer::clean()
	{
		glCheck(glDeleteBuffers(1, &m_id));
		return (*this);
	}

	VertexBuffer & VertexBuffer::bind()
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		return (*this);
	}

	VertexBuffer & VertexBuffer::unbind()
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
		return (*this);
	}
}