#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

#include <MemeGraphics/RenderBatch.hpp>

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

#endif // !_DRAWABLE_HPP_
