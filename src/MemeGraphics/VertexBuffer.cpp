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

	VertexBuffer & VertexBuffer::create()
	{
		m_id = OpenGL::genBuffers(1);
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


	VertexBuffer & VertexBuffer::update(GL::Usage usage, const std::vector<float>& data)
	{
		return update(usage, &data[0], (uint32_t)data.size());
	}

	VertexBuffer & VertexBuffer::update(GL::Usage usage, const void * data, uint32_t size)
	{
		m_usage = usage;
		m_data = data;
		m_size = size;

		switch (m_usage)
		{
		case GL::StaticDraw:
			OpenGL::bufferData(
				GL::ArrayBuffer,
				(sizeof(float) * m_size),
				m_data,
				m_usage);
			break;

		case GL::DynamicDraw:
			OpenGL::bufferSubData(
				GL::ArrayBuffer,
				0,
				(sizeof(float) * m_size),
				m_data);
			break;

		case GL::StreamDraw:
		default:
			break;
		}
		return (*this);
	}
}