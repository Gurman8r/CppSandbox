#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/Vertex.h>

namespace ml
{
	VertexBuffer::VertexBuffer()
		: IHandle()
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & copy)
		: IHandle(copy.m_id)
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
		if (*this)
		{
			OpenGL::deleteBuffers(1, (*this));
		}
		return (*this);
	}

	VertexBuffer & VertexBuffer::create(GL::Usage usage)
	{
		if (!(*this) && (m_id = OpenGL::genBuffers(1)))
		{
			m_usage = usage;
		}
		return (*this);
	}


	VertexBuffer & VertexBuffer::bind()
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, (*this));
		return (*this);
	}

	VertexBuffer & VertexBuffer::unbind()
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, NULL);
		return (*this);
	}


	VertexBuffer & VertexBuffer::bufferData(const void * data, uint32_t size)
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		OpenGL::bufferData(GL::ArrayBuffer, (sizeof(float) * m_size), m_data, m_usage);
		return (*this);
	}

	VertexBuffer & VertexBuffer::bufferData(const FloatList & data)
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}


	VertexBuffer & VertexBuffer::bufferSubData(const void * data, uint32_t size, uint32_t offset)
	{
		m_data = data;
		m_size = size;
		m_count = size / Vertex::Size;
		OpenGL::bufferSubData(GL::ArrayBuffer, offset, (sizeof(float) * m_size), m_data);
		return (*this);
	}

	VertexBuffer & VertexBuffer::bufferSubData(const FloatList & data, uint32_t offset)
	{
		return bufferSubData(&data[0], data.size(), offset);
	}

}