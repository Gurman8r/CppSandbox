#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	VertexArray & VertexArray::clean()
	{
		if ((*this))
		{
			ML_GL.deleteVertexArrays(1, (*this));
		}
		return (*this);
	}

	VertexArray & VertexArray::create(GL::Mode mode)
	{
		if (set_handle(ML_GL.genVertexArrays(1)))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	const VertexArray & VertexArray::bind() const
	{
		ML_GL.bindVertexArray((*this));
		return (*this);
	}

	const VertexArray & VertexArray::unbind() const
	{
		ML_GL.bindVertexArray(NULL);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

}