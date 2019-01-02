#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/Vector2.h>

namespace ml
{
	struct ML_GRAPHICS_API Glyph final
		: public ITrackable
	{
		uint32_t	texture;
		vec2i		position;
		vec2i		size;
		uint32_t	advance;
	};
}

#endif // !_GLYPH_H_
