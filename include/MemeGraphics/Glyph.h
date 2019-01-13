#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <MemeGraphics/Texture.h>
#include <MemeCore/Rect.h>

namespace ml
{
	struct ML_GRAPHICS_API Glyph final
		: public ITrackable
	{
		enum { VertexCount = 6U };

		Texture		texture;
		FloatRect	bounds;
		uint32_t	advance;

		inline const vec2f bearing()	const { return bounds.position(); }
		inline const vec2f size()		const { return bounds.size(); }
		inline const float x()			const { return bearing()[0]; }
		inline const float y()			const { return bearing()[1]; }
		inline const float width()		const { return size()[0]; }
		inline const float height()		const { return size()[1]; }
		inline const vec2f offset()		const { return vec2f(x(), -(height() - y())); }
		inline const float step()		const { return (float)(advance >> VertexCount); }
	};
}

#endif // !_GLYPH_H_
