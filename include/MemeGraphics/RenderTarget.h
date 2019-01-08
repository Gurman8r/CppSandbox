#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>

namespace ml
{
	class ML_GRAPHICS_API RenderTarget
	{
	public:
		virtual ~RenderTarget() {}

	public: // Drawing
		RenderTarget & clear();
		RenderTarget & clear(const vec4f & color);

		RenderTarget & draw(const IDrawable & value);
		RenderTarget & draw(const RenderState & value);

		RenderTarget & drawElements(const IBO & ibo, GL::Primitive prim, GL::Type type);

	public:// Utility
		bool getFlag(GL::Base value) const;
		void setFlag(GL::Base flag, bool value) const;

		void enableFlag(GL::Base value) const;
		void disableFlag(GL::Base value) const;

		void setActiveTexture(GL::TextureID textureID) const;
		void setAlphaFunc(GL::Comparison comp, float value) const;
		void setBlendFunc(GL::Factor src, GL::Factor dst) const;
		void setCullFace(GL::Face value) const;
		void setDepthFunc(GL::Comparison value) const;
		void setViewport(const vec2i & pos, const vec2i & size) const;


	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
