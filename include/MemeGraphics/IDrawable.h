#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <MemeGraphics/RenderState.h>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API IDrawable
	{
	public:
		virtual ~IDrawable() {}

		virtual void draw(RenderTarget & target, RenderState state) const = 0;

	private:

	};
}

#endif // !_DRAWABLE_H_
