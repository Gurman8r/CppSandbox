#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
		: IHandle()
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: IHandle(copy.m_id)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}


	VertexArray & VertexArray::clean()
	{
		if (*this)
		{
			OpenGL::deleteVertexArrays(1, (*this));
		}
		return (*this);
	}

	VertexArray & VertexArray::create(GL::Mode mode)
	{
		if (!(*this) && (m_id = OpenGL::genVertexArrays(1)))
		{
			m_mode = mode;
		}
		return (*this);
	}
	

	VertexArray & VertexArray::bind()
	{
		OpenGL::bindVertexArray((*this));
		return (*this);
	}

	VertexArray & VertexArray::unbind()
	{
		OpenGL::bindVertexArray(NULL);
		return (*this);
	}

}