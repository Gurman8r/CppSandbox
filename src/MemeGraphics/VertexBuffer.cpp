#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Vertex.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	VertexBuffer::VertexBuffer()
		: IHandle(NULL)
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & copy)
		: IHandle(copy)
		, m_usage(copy.m_usage)
		, m_data(copy.m_data)
		, m_size(copy.m_size)
		, m_count(copy.m_count)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		clean();
	}


	VertexBuffer & VertexBuffer::clean()
	{
		if ((*this))
		{
			OpenGL::deleteBuffers(1, (*this));
		}
		return (*this);
	}

	VertexBuffer & VertexBuffer::create(GL::Usage usage)
	{
		if (!(*this) && (get_ref() = OpenGL::genBuffers(1)))
		{
			m_usage = usage;
		}
		return (*this);
	}


	void VertexBuffer::bind() const
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, (*this));
	}
	
	void VertexBuffer::unbind() const
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, NULL);
	}
	
	
	void VertexBuffer::bufferData(const void * data, uint32_t size) const
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		OpenGL::bufferData(GL::ArrayBuffer, (sizeof(float) * m_size), m_data, m_usage);
	}
	
	void VertexBuffer::bufferData(const FloatList & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}
	
	void VertexBuffer::bufferSubData(const void * data, uint32_t size, uint32_t offset) const
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		OpenGL::bufferSubData(GL::ArrayBuffer, offset, (sizeof(float) * m_size), m_data);
	}
	
	void VertexBuffer::bufferSubData(const FloatList & data, uint32_t offset) const
	{
		return bufferSubData(&data[0], (uint32_t)data.size(), offset);
	}

}