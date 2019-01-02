#ifndef _TEXT_H_
#define _TEXT_H_

#include <MemeGraphics/IDrawable.h>

namespace ml
{
	class ML_GRAPHICS_API Text
		: public ITrackable
		, public IDrawable
	{
	public:
		Text();
		~Text();

	private:

	};

	
}

#endif // !_TEXT_H_
