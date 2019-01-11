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

		RenderTarget & drawElements(const IBO & ibo, GL::Mode prim, GL::Type type);

	public:
		bool isEnabled(GL::Enum value) const;
		bool enable(GL::Enum value, bool check = false) const;
		bool disable(GL::Enum value, bool check = false) const;

		void activeTexture(GL::TextureID textureID) const;
		void alphaFunc(GL::Comparison comp, float value) const;
		void blendFunc(GL::Factor src, GL::Factor dst) const;
		void cullFace(GL::Face value) const;
		void depthFunc(GL::Comparison value) const;
		void viewport(const vec2i & pos, const vec2i & size) const;


	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
