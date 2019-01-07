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
		glCheck(glBufferData(GL_ARRAY_BUFFER, (size * sizeof(float)), data, m_usage));
	}

	VertexBuffer::VertexBuffer(Enum::Usage usage, const std::vector<float>& data)
		: VertexBuffer(usage, &data[0], data.size())
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
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::unbind() const
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}