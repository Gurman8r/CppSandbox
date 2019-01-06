#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/ITransformable.h>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API Sprite final
		: public ITrackable
		, public IDrawable
		, public ITransformable
	{
	public:
		Sprite();
		~Sprite();

		void draw(RenderTarget * target, RenderState state) const;

	private:

	};
}

#endif // !_SPRITE_H_
