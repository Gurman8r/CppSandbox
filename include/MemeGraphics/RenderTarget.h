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


	public:
		void setViewport(const vec2i & pos, const vec2i & size) const;

	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
