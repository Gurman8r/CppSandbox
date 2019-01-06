#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	RenderTarget & RenderTarget::clear(const vec4f & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		return (*this);
	}

	RenderTarget & RenderTarget::draw(const IDrawable * value)
	{
		value->draw(this, RenderState());
		return (*this);
	}

	RenderTarget & RenderTarget::draw(RenderState & value)
	{
		return (*this);
	}
}