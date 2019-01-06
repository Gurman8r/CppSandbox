#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/Rect.h>

namespace ml
{
	struct ML_GRAPHICS_API Glyph final
		: public ITrackable
	{
		uint32_t	texture;
		IntRect		bounds;
		uint32_t	advance;
	};
}

#endif // !_GLYPH_H_
