#ifndef _RENDER_TARGET_HPP_
#define _RENDER_TARGET_HPP_

#include <MemeGraphics/IRenderer.hpp>
#include <MemeGraphics/VertexArray.hpp>
#include <MemeGraphics/VertexBuffer.hpp>
#include <MemeGraphics/IndexBuffer.hpp>
#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/RenderBuffer.hpp>

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
		RenderTarget & draw(VAO & vao, VBO & vbo, IBO & ibo);

	public:
		void setViewport(const vec2i & pos, const vec2i & size) const;

	private:
	};

	
}

#endif // !_RENDER_TARGET_HPP_
