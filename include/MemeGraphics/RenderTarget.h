#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <MemeGraphics/RenderState.h>
#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/VertexBuffer.h>
#include <MemeGraphics/IndexBuffer.h>
#include <MemeGraphics/Enum.h>

namespace ml
{
	class ML_GRAPHICS_API RenderTarget
	{
	public:
		virtual ~RenderTarget() {}

		RenderTarget & clear();
		RenderTarget & clear(const vec4f & value);

		RenderTarget & draw(const IDrawable & value);
		RenderTarget & draw(const RenderState & value);
		RenderTarget & draw(const VertexArray & vertices, const RenderState & state);
		RenderTarget & draw(const Vertex * vertices, std::size_t count, Enum::Primitive type, const RenderState & state);
		RenderTarget & draw(const VertexBuffer & buffer, std::size_t begin, std::size_t count, const RenderState & state);

		RenderTarget & drawElements(const IBO & ibo, Enum::Primitive primitive, Enum::Type type);
		RenderTarget & drawElements(Enum::Primitive primitive, Enum::Type type, std::size_t count, const float * indices);

		RenderTarget & enableFlag(Enum::Flag value);
		RenderTarget & disableFlag(Enum::Flag value);

		bool isEnabled(Enum::Flag value) const;


	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
