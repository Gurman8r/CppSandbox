#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexBuffer::VertexBuffer()
		: m_id(NULL)
	{
	}

	VertexBuffer::VertexBuffer(GL::Usage usage, const void * data, uint32_t size)
	{
		create(usage, data, size);
	}

	VertexBuffer::VertexBuffer(GL::Usage usage, const std::vector<float>& data)
		: VertexBuffer(usage, &data[0], (uint32_t)data.size())
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

	VertexBuffer & VertexBuffer::create(GL::Usage usage, const void * data, uint32_t size)
	{
		m_usage = usage;
		m_data	= data;
		m_size	= size;
		m_id	= OpenGL::genBuffers(1);

		bind();

		return update();
	}

	VertexBuffer & VertexBuffer::create(GL::Usage usage, const std::vector<float>& data)
	{
		return create(usage, &data[0], (uint32_t)data.size());
	}

	VertexBuffer & VertexBuffer::update()
	{
		OpenGL::bufferData(
			GL::ArrayBuffer,
			(m_size * sizeof(float)),
			m_data,
			m_usage);

		return (*this);
	}


	void VertexBuffer::bind() const
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, m_id);
	}

	void VertexBuffer::unbind() const
	{
		OpenGL::bindBuffer(GL::ArrayBuffer, NULL);
	}
}