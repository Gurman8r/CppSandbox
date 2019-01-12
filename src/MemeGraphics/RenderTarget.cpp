#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	// Drawing
	
	RenderTarget & RenderTarget::clear()
	{
		OpenGL::clear(GL::DepthBufferBit);
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4f & color)
	{
		OpenGL::clearColor(color[0], color[1], color[2], color[3]);
		OpenGL::clear(GL::ColorBufferBit | GL::DepthBufferBit);
		return (*this);
	}
	

	RenderTarget & RenderTarget::draw(const IDrawable & value)
	{
		value.draw((*this), RenderState());
		return (*this);
	}

	RenderTarget & RenderTarget::draw(const RenderState & value)
	{
		return (*this);
	}
	
	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		OpenGL::viewport(pos[0], pos[1], size[0], size[1]);
	}
}