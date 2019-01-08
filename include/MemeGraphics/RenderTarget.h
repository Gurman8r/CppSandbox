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

		RenderTarget & drawElements(const IBO & ibo, Enum::Primitive prim, Enum::Type type);

	public:// Utility
		bool getFlag(Enum::Base value) const;
		void setFlag(Enum::Base flag, bool value) const;

		void enableFlag(Enum::Base value) const;
		void disableFlag(Enum::Base value) const;

		void setActiveTexture(Enum::TextureID textureID) const;
		void setAlphaFunc(Enum::Comparison comp, float value) const;
		void setBlendFunc(Enum::Factor src, Enum::Factor dst) const;
		void setCullFace(Enum::Face value) const;
		void setDepthFunc(Enum::Comparison value) const;
		void setViewport(const vec2i & pos, const vec2i & size) const;


	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
