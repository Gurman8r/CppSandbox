#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer(Enum::Usage usage, const uint32_t * data, uint32_t size)
		: m_usage(usage)
		, m_data(data)
		, m_count(size)
	{
		glCheck(glGenBuffers(1, &m_id));
		bind();
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (size * sizeof(uint32_t)), data, m_usage));
	}

	IndexBuffer::IndexBuffer(Enum::Usage usage, const std::vector<uint32_t>& data)
		: IndexBuffer(usage, &data[0], data.size())
	{
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


	void IndexBuffer::bind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void IndexBuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}