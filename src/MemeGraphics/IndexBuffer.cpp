#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	IndexBuffer::IndexBuffer()
		: m_id(NULL)
	{
	}

	IndexBuffer::IndexBuffer(const IndexBuffer & copy)
		: m_id(copy.m_id)
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
		OpenGL::deleteBuffers(1, &m_id);
		return (*this);
	}

	IndexBuffer & IndexBuffer::create()
	{
		m_id = OpenGL::genBuffers(1);
		return (*this);
	}


	IndexBuffer & IndexBuffer::bind()
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, m_id);
		return (*this);
	}

	IndexBuffer & IndexBuffer::unbind()
	{
		OpenGL::bindBuffer(GL::ElementArrayBuffer, NULL);
		return (*this);
	}


	IndexBuffer & IndexBuffer::update(GL::Usage usage, const std::vector<uint32_t>& data)
	{
		return update(usage, &data[0], (uint32_t)data.size());
	}

	IndexBuffer & IndexBuffer::update(GL::Usage usage, const uint32_t * data, uint32_t count)
	{
		m_usage = usage;
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