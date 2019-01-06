#ifndef _TEXT_H_
#define _TEXT_H_

#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/ITransformable.h>
#include <MemeGraphics/Font.h>

namespace ml
{
	class ML_GRAPHICS_API Text final
		: public ITrackable
		, public IDrawable
		, public ITransformable
	{
	public:
		Text();
		~Text();

		void draw(RenderTarget * target, RenderState state) const;

	private:

	};

	
}

#endif // !_TEXT_H_
