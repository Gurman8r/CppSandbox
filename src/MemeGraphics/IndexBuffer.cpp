#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer(Enum::Usage usage, const uint32_t * data, uint32_t count)
		: m_usage(usage)
		, m_data(data)
		, m_count(count)
	{
		glCheck(glGenBuffers(1, &m_id));
		
		bind();
		
		glCheck(glBufferData(
			Enum::ElementArrayBuffer, 
			(m_count * sizeof(uint32_t)),
			m_data, 
			m_usage));
	}

	IndexBuffer::IndexBuffer(Enum::Usage usage, const std::vector<uint32_t>& data)
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
		glCheck(glBindBuffer(Enum::ElementArrayBuffer, m_id));
	}

	void IndexBuffer::unbind() const
	{
		glCheck(glBindBuffer(Enum::ElementArrayBuffer, 0));
	}
}