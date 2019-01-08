#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/GLEW.h>

namespace ml
{
	RenderTarget & RenderTarget::clear()
	{
		glCheck(glClear(GL::DepthBufferBit));
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4f & color)
	{
		glCheck(glClearColor(color[0], color[1], color[2], color[3]));
		glCheck(glClear(GL::ColorBufferBit | GL::DepthBufferBit));
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


	RenderTarget & RenderTarget::drawElements(const IBO & ibo, GL::Primitive prim, GL::Type type)
	{
		ibo.bind();
		glCheck(glDrawElements(prim, (uint32_t)ibo.size(), type, NULL));
		return (*this);
	}
	
	
	
	bool RenderTarget::getFlag(GL::Enum value) const
	{
		static bool tmp;
		glCheck(tmp = glIsEnabled(value));
		return tmp;
	}

	void RenderTarget::setFlag(GL::Enum flag, bool value) const
	{
		if (value)
		{
			enableFlag(flag);
		}
		else
		{
			disableFlag(flag);
		}
	}
		
	void RenderTarget::enableFlag(GL::Enum value) const
	{
		glCheck(glEnable(value));
	}

	void RenderTarget::disableFlag(GL::Enum value) const
	{
		glCheck(glDisable(value));
	}
	

	void RenderTarget::setActiveTexture(GL::TextureID textureID) const
	{
		glCheck(glActiveTexture(textureID));
	}

	void RenderTarget::setAlphaFunc(GL::Comparison cmp, float value) const
	{
		glCheck(glAlphaFunc(cmp, value));
	}

	void RenderTarget::setBlendFunc(GL::Factor src, GL::Factor dst) const
	{
		glCheck(glBlendFunc(src, dst));
	}

	void RenderTarget::setCullFace(GL::Face face) const
	{
		glCheck(glCullFace(face));
	}

	void RenderTarget::setDepthFunc(GL::Comparison cmp) const
	{
		glCheck(glDepthFunc(cmp));
	}
	
	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		glCheck(glViewport(pos[0], pos[1], size[0], size[1]));
	}
}