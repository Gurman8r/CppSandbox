#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer()
	{
	}

	IndexBuffer::IndexBuffer(GL::Usage usage, const uint32_t * data, uint32_t count)
		: m_usage(usage)
		, m_data(data)
		, m_count(count)
	{
		m_id = OpenGL::genBuffers(1);
		
		bind();
		
		OpenGL::bufferData(
			GL::ElementArrayBuffer, 
			(m_count * sizeof(uint32_t)),
			m_data, 
			m_usage);
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
		OpenGL::deleteBuffers(1, &m_id);
		return (*this);
	}


	void IndexBuffer::bind() const
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, m_id);
	}

	void IndexBuffer::unbind() const
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, 0);
	}
}