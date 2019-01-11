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
	

	RenderTarget & RenderTarget::drawElements(const IBO & ibo, GL::Mode mode, GL::Type type)
	{
		if (ibo)
		{
			OpenGL::drawElements(mode, (int32_t)ibo.count(), type, NULL);
		}
		return (*this);
	}
	

	// Flags
	
	bool RenderTarget::isEnabled(GL::Enum value) const
	{
		return OpenGL::isEnabled(value);
	}
		
	bool RenderTarget::enable(GL::Enum value, bool check) const
	{
		return OpenGL::enable(value, check);
	}

	bool RenderTarget::disable(GL::Enum value, bool check) const
	{
		return OpenGL::disable(value, check);
	}
	

	// Functions

	void RenderTarget::activeTexture(GL::TextureID target) const
	{
		OpenGL::activeTexture(target);
	}

	void RenderTarget::alphaFunc(GL::Comparison cmp, float value) const
	{
		OpenGL::alphaFunc(cmp, value);
	}

	void RenderTarget::blendFunc(GL::Factor src, GL::Factor dst) const
	{
		OpenGL::blendFunc(src, dst);
	}

	void RenderTarget::cullFace(GL::Face face) const
	{
		OpenGL::cullFace(face);
	}

	void RenderTarget::depthFunc(GL::Comparison cmp) const
	{
		OpenGL::depthFunc(cmp);
	}
	
	void RenderTarget::viewport(const vec2i & pos, const vec2i & size) const
	{
		OpenGL::viewport(pos[0], pos[1], size[0], size[1]);
	}
}