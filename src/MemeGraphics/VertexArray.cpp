#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
		: m_id(NULL)
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: m_id(copy.m_id)
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

	VertexArray & VertexArray::create()
	{
		m_id = OpenGL::genVertexArrays(1);
		return (*this);
	}
	

	VertexArray & VertexArray::bind()
	{
		OpenGL::bindVertexArray(m_id);
		return (*this);
	}

	VertexArray & VertexArray::unbind()
	{
		OpenGL::bindVertexArray(NULL);
		return (*this);
	}


	VertexArray & VertexArray::update()
	{
		return (*this);
	}

}