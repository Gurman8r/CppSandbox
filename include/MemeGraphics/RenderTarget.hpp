#ifndef _RENDER_TARGET_HPP_
#define _RENDER_TARGET_HPP_

#include <MemeGraphics/IDrawable.hpp>
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

	public:
		RenderTarget & clear();
		RenderTarget & clear(const vec4f & color);

	public:
		RenderTarget & draw(const IDrawable & value);
		RenderTarget & draw(const IDrawable & value, const RenderBatch & batch);
		RenderTarget & draw(const RenderBatch & value);
		RenderTarget & draw(const VAO * vao, const VBO * vbo, const IBO * ibo);
		RenderTarget & draw(const VAO * vao, const VBO * vbo);

	public:
		void setViewport(const vec2i & pos, const vec2i & size) const;

	private:
	};

	
}

#endif // !_RENDER_TARGET_HPP_
