#ifndef _ML_GLYPH_HPP_
#define _ML_GLYPH_HPP_

#include <MemeGraphics/Shapes.hpp>

namespace ml
{
	struct ML_GRAPHICS_API Glyph final
		: public ITrackable
	{
		enum { VertexCount = 6U };

		Texture		texture;
		FloatRect	bounds;
		uint32_t	advance;

		Glyph()
			: texture(GL::Texture2D, GL::RGBA, GL::Red, true, true)
		{
		}

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

#endif // !_ML_GLYPH_HPP_