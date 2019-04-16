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
		if (!(*this) && (get_ref() = ML_GL.genVertexArrays(1)))
		{
			m_mode = mode;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void VertexArray::bind() const
	{
		ML_GL.bindVertexArray((*this));
	}

	void VertexArray::unbind() const
	{
		ML_GL.bindVertexArray(NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * */

}