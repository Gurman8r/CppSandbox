#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <MemeGraphics/IDrawable.h>

namespace ml
{
	class ML_GRAPHICS_API Sprite
		: public ITrackable
		, public IDrawable
	{
	public:
		Sprite();
		~Sprite();

	private:

	};
}

#endif // !_SPRITE_H_
