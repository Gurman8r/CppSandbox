#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	RenderTarget & RenderTarget::clear()
	{
		glCheck(glClear(GL_DEPTH_BUFFER_BIT));
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4f & value)
	{
		glCheck(glClearColor(value[0], value[1], value[2], value[3]));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
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

	RenderTarget & RenderTarget::draw(const VertexArray & vertices, const RenderState & state)
	{
		return draw(vertices.ptr(), vertices.count(), vertices.primitive(), state);
	}

	RenderTarget & RenderTarget::draw(const Vertex * vertices, std::size_t count, Enum::Primitive type, const RenderState & state)
	{
		return (*this);
	}

	RenderTarget & RenderTarget::draw(const VertexBuffer & buffer, std::size_t begin, std::size_t count, const RenderState & state)
	{
		return (*this);
	}


	RenderTarget & RenderTarget::drawElements(const IBO & ibo, Enum::Primitive primitive, Enum::Type type)
	{
		ibo.bind();
		return drawElements(primitive, type, ibo.size(), NULL);
	}

	RenderTarget & RenderTarget::drawElements(Enum::Primitive primitive, Enum::Type type, std::size_t count, const float * indices)
	{
		glCheck(glDrawElements(primitive, count, type, indices));
		return (*this);
	}
	
	
	RenderTarget & RenderTarget::enableFlag(Enum::Flag value)
	{
		glCheck(glEnable(static_cast<GLenum>(value)));
		return (*this);
	}

	RenderTarget & RenderTarget::disableFlag(Enum::Flag value)
	{
		glCheck(glDisable(static_cast<GLenum>(value)));
		return (*this);
	}
	
	
	bool RenderTarget::isEnabled(Enum::Flag value) const
	{
		return (glIsEnabled(value) == GL_TRUE);
	}
}