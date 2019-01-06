#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer(Usage usage, const uint32_t * data, uint32_t count)
		: m_usage(usage)
	{
		glCheck(glGenBuffers(1, &m_id));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (count * sizeof(uint32_t)), data, static_cast<GLenum>(m_usage)));
	}

	IndexBuffer::~IndexBuffer()
	{
		clean();
	}

	IndexBuffer & IndexBuffer::clean()
	{
		glCheck(glDeleteBuffers(1, &m_id));
		return (*this);
	}

	IndexBuffer & IndexBuffer::bind()
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
		return (*this);
	}

	IndexBuffer & IndexBuffer::unbind()
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		return (*this);
	}
}