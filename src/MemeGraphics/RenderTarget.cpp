#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	RenderTarget & RenderTarget::clear()
	{
		glCheck(glClear(Enum::DepthBufferBit));
		return (*this);
	}
	
	RenderTarget & RenderTarget::clear(const vec4f & color)
	{
		glCheck(glClearColor(color[0], color[1], color[2], color[3]));
		glCheck(glClear(Enum::ColorBufferBit | Enum::DepthBufferBit));
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


	RenderTarget & RenderTarget::drawElements(const IBO & ibo, Enum::Primitive prim, Enum::Type type)
	{
		ibo.bind();
		glCheck(glDrawElements(prim, (uint32_t)ibo.size(), type, NULL));
		return (*this);
	}
	
	
	
	bool RenderTarget::getFlag(Enum::Base value) const
	{
		static bool tmp;
		glCheck(tmp = glIsEnabled(value));
		return tmp;
	}

	void RenderTarget::setFlag(Enum::Base flag, bool value) const
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
		
	void RenderTarget::enableFlag(Enum::Base value) const
	{
		glCheck(glEnable(value));
	}

	void RenderTarget::disableFlag(Enum::Base value) const
	{
		glCheck(glDisable(value));
	}
	

	void RenderTarget::setActiveTexture(Enum::TextureID textureID) const
	{
		glCheck(glActiveTexture(textureID));
	}

	void RenderTarget::setAlphaFunc(Enum::Comparison cmp, float value) const
	{
		glCheck(glAlphaFunc(cmp, value));
	}

	void RenderTarget::setBlendFunc(Enum::Factor src, Enum::Factor dst) const
	{
		glCheck(glBlendFunc(src, dst));
	}

	void RenderTarget::setCullFace(Enum::Face face) const
	{
		glCheck(glCullFace(face));
	}

	void RenderTarget::setDepthFunc(Enum::Comparison cmp) const
	{
		glCheck(glDepthFunc(cmp));
	}
	
	void RenderTarget::setViewport(const vec2i & pos, const vec2i & size) const
	{
		glCheck(glViewport(pos[0], pos[1], size[0], size[1]));
	}
}