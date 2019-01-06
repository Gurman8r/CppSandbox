#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <MemeGraphics/RenderState.h>
#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/Primitive.h>
#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/VertexBuffer.h>

namespace ml
{
	class ML_GRAPHICS_API RenderTarget
	{
	public:
		enum
		{
			DepthTest	= 0x0B71,
			CullFace	= 0x0B44,
		};
	public:
		virtual ~RenderTarget() {}

		RenderTarget & clear(const vec4f & value);
		RenderTarget & enable(uint32_t value);
		RenderTarget & disable(uint32_t value);

		RenderTarget & draw(const IDrawable & value);
		RenderTarget & draw(const RenderState & value);
		RenderTarget & draw(const VertexArray & vertices, const RenderState & state);
		RenderTarget & draw(const Vertex * vertices, std::size_t count, Primitive::Type type, const RenderState & state);
		RenderTarget & draw(const VertexBuffer & buffer, std::size_t begin, std::size_t count, const RenderState & state);



	private:
	};

	
}

#endif // !_RENDER_TARGET_H_
