#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	VertexArray::VertexArray()
		: IHandle(NULL)
	{
	}

	VertexArray::VertexArray(const VertexArray & copy)
		: IHandle(copy)
	{
	}

	VertexArray::~VertexArray()
	{
		clean();
	}


	VertexArray & VertexArray::clean()
	{
		if ((*this))
		{
			OpenGL::deleteVertexArrays(1, (*this));
		}
		return (*this);
	}

	VertexArray & VertexArray::create(GL::Mode mode)
	{
		if (!(*this) && (get_ref() = OpenGL::genVertexArrays(1)))
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