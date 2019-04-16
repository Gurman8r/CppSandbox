#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	IndexBuffer::IndexBuffer()
		: IHandle	(NULL)
		, m_data	(NULL)
		, m_count	(NULL)
		, m_usage	(GL::StaticDraw)
		, m_type	(GL::UnsignedInt)
	{
	}

	IndexBuffer::IndexBuffer(const IndexBuffer & copy)
		: IHandle	(copy)
		, m_data	(copy.m_data)
		, m_count	(copy.m_count)
		, m_usage	(copy.m_usage)
		, m_type	(copy.m_type)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	IndexBuffer & IndexBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteBuffers(1, (*this));
		}
		return (*this);
	}

	IndexBuffer & IndexBuffer::create(GL::Usage usage, GL::Type type)
	{
		if (!(*this) && (get_ref() = ML_GL.genBuffers(1)))
		{
			m_usage = usage;
			m_type = type;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void IndexBuffer::bind() const
	{
		ML_GL.bindBuffer(GL::ElementArrayBuffer, (*this));
	}

	void IndexBuffer::unbind() const
	{
		ML_GL.bindBuffer(GL::ElementArrayBuffer, NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void IndexBuffer::bufferData(const IndexList & data) const
	{
		return bufferData(&data[0], (uint32_t)data.size());
	}

	void IndexBuffer::bufferData(const uint32_t * data, uint32_t count) const
	{
		m_data = data;
		m_count = count;

		ML_GL.bufferData(
			GL::ElementArrayBuffer,
			(m_count * sizeof(uint32_t)),
			m_data,
			m_usage);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}