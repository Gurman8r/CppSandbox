#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include <MemeGraphics/RenderState.h>
#include <MemeGraphics/IDrawable.h>

namespace ml
{
	class ML_GRAPHICS_API RenderTarget
	{
	public:
		virtual ~RenderTarget() {}

		RenderTarget & clear(const vec4f & value);
		RenderTarget & draw(const IDrawable * value);
		RenderTarget & draw(RenderState & value);

	private:

	};

	
}

#endif // !_RENDER_TARGET_H_
