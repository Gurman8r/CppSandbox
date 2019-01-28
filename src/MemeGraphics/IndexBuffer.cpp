#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer()
		: IHandle(NULL)
	{
	}

	IndexBuffer::IndexBuffer(const IndexBuffer & copy)
		: IHandle(copy)
		, m_usage(copy.m_usage)
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
		if ((*this))
		{
			OpenGL::deleteBuffers(1, (*this));
		}
		return (*this);
	}

	IndexBuffer & IndexBuffer::create(GL::Usage usage, GL::Type type)
	{
		if (!(*this) && (handle() = OpenGL::genBuffers(1)))
		{
			m_usage = usage;
			m_type = type;
		}
		return (*this);
	}


	IndexBuffer & IndexBuffer::bind()
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, (*this));
		return (*this);
	}

	IndexBuffer & IndexBuffer::unbind()
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, NULL);
		return (*this);
	}


	IndexBuffer & IndexBuffer::bufferData(const IndexList & data)
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}

	IndexBuffer & IndexBuffer::bufferData(const uint32_t * data, uint32_t count)
	{
		m_data = data;
		m_count = count;

		OpenGL::bufferData(
			GL::ElementArrayBuffer,
			(m_count * sizeof(uint32_t)),
			m_data,
			m_usage);

		return (*this);
	}
}