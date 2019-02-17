#ifndef _I_DRAWABLE_HPP_
#define _I_DRAWABLE_HPP_

#include <MemeGraphics/RenderBatch.hpp>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API IDrawable
	{
	public:
		virtual ~IDrawable() {}

		virtual void draw(RenderTarget & target, RenderBatch batch) const = 0;

	private:

	};
}

#endif // !_I_DRAWABLE_HPP_