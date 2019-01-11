#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
		: m_id(NULL)
	{
	}

	VertexArray::VertexArray(uint32_t count)
	{
		create(count);
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: m_id(copy.m_id)
		, m_count(copy.m_count)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}


	VertexArray & VertexArray::clean()
	{
		return (*this);
	}

	VertexArray & VertexArray::create(uint32_t count)
	{
		m_count = count;
		m_id = OpenGL::genVertexArrays(m_count);

		bind();

		return update();
	}

	VertexArray & VertexArray::update()
	{
		return (*this);
	}


	void VertexArray::bind() const
	{
		OpenGL::bindVertexArray(m_id);
	}

	void VertexArray::unbind() const
	{
		OpenGL::bindVertexArray(NULL);
	}
}