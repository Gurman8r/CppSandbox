#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_

#include <MemeGraphics/RenderBatch.h>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API IRenderer
	{
	public:
		virtual ~IRenderer() {}

		virtual void draw(RenderTarget & target, RenderBatch batch) const = 0;

	private:

	};
}

#endif // !_DRAWABLE_H_
