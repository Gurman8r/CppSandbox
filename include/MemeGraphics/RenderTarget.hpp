#ifndef _ML_RENDER_TARGET_HPP_
#define _ML_RENDER_TARGET_HPP_

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

		void setViewport(const vec2i & pos, const vec2i & size) const;

	public:
		RenderTarget & draw(const IDrawable & value);
		RenderTarget & draw(const IDrawable & value, const RenderBatch & batch);
		
		RenderTarget & draw(const VertexList & vertices, const RenderBatch & batch);
		RenderTarget & draw(const FloatList & vertices, const RenderBatch & batch);
		RenderTarget & draw(const float * vertices, size_t vertexCount, const RenderBatch & batch);
		
		RenderTarget & draw(const VAO & vao, const VBO & vbo, const IBO & ibo);
		RenderTarget & draw(const VAO & vao, const VBO & vbo);

	};

	
}

#endif // !_ML_RENDER_TARGET_HPP_