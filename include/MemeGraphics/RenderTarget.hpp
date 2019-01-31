#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <MemeGraphics/IRenderer.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>

namespace ml
{
	class ML_GRAPHICS_API RenderTarget
	{
	public:
		virtual ~RenderTarget() {}

	public: // Drawing
		RenderTarget & clear();
		RenderTarget & clear(const vec4f & color);

		RenderTarget & draw(const IRenderer & value);
		RenderTarget & draw(const IRenderer & value, const RenderBatch & batch);
		RenderTarget & draw(const RenderBatch & value);


	public:
		void setViewport(const vec2i & pos, const vec2i & size) const;

	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
