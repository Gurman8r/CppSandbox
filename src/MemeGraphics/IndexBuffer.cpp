#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer(GL::Usage usage, const uint32_t * data, uint32_t count)
		: m_usage(usage)
		, m_data(data)
		, m_count(count)
	{
		glCheck(glGenBuffers(1, &m_id));
		
		bind();
		
		glCheck(glBufferData(
			GL::ElementArrayBuffer, 
			(m_count * sizeof(uint32_t)),
			m_data, 
			m_usage));
	}

	IndexBuffer::IndexBuffer(GL::Usage usage, const std::vector<uint32_t>& data)
		: IndexBuffer(usage, &data[0], (uint32_t)data.size())
	{
	}

	IndexBuffer::IndexBuffer(const IndexBuffer & copy)
		: m_usage(copy.m_usage)
		, m_data(copy.m_data)
		, m_count(copy.m_count)
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
		glCheck(glBindBuffer(GL::ElementArrayBuffer, m_id));
	}

	void IndexBuffer::unbind() const
	{
		glCheck(glBindBuffer(GL::ElementArrayBuffer, 0));
	}
}