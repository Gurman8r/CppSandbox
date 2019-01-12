#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexBuffer::VertexBuffer()
		: m_id(NULL)
	{
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & copy)
		: m_id(copy.m_id)
		, m_usage(copy.m_usage)
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
		OpenGL::deleteBuffers(1, &m_id);
		return (*this);
	}

	VertexBuffer & VertexBuffer::create(GL::Usage usage)
	{
		m_id = OpenGL::genBuffers(1);
		m_usage = usage;
		return (*this);
	}


	VertexBuffer & VertexBuffer::bind()
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, m_id);
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
		OpenGL::bufferData(GL::ArrayBuffer, (sizeof(float) * m_size), m_data, m_usage);
		return (*this);
	}

	VertexBuffer & VertexBuffer::bufferData(const std::vector<float>& data)
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}


	VertexBuffer & VertexBuffer::bufferSubData(const void * data, uint32_t size)
	{
		m_data = data;
		m_size = size;
		OpenGL::bufferSubData(GL::ArrayBuffer, 0, (sizeof(float) * m_size), m_data);
		return (*this);
	}

	VertexBuffer & VertexBuffer::bufferSubData(const std::vector<float>& data)
	{
		return bufferSubData(&data[0], data.size());
	}

}