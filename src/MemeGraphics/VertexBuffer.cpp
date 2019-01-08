#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexBuffer::VertexBuffer(Enum::Usage usage, const void * data, uint32_t size)
		: m_usage(usage)
		, m_data(data)
		, m_size(size)
	{
		glCheck(glGenBuffers(1, &m_id));
		
		bind();
		
		glCheck(glBufferData(
			Enum::ArrayBuffer, 
			(m_size * sizeof(float)),
			m_data, 
			m_usage));
	}

	VertexBuffer::VertexBuffer(Enum::Usage usage, const std::vector<float>& data)
		: VertexBuffer(usage, &data[0], (uint32_t)data.size())
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & copy)
		: m_usage(copy.m_usage)
		, m_data(copy.m_data)
		, m_size(copy.m_size)
	{
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


	void VertexBuffer::bind() const
	{
		glCheck(glBindBuffer(Enum::ArrayBuffer, m_id));
	}

	void VertexBuffer::unbind() const
	{
		glCheck(glBindBuffer(Enum::ArrayBuffer, 0));
	}
}