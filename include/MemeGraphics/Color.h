#ifndef _COLOR_H_
#define _COLOR_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/Vector4.h>

namespace ml
{
	class ML_GRAPHICS_API Color
	{
	public:
		const static vec4f Clear;
		const static vec4f White;
		const static vec4f Black;

		const static vec4f Red;
		const static vec4f Blue;
		const static vec4f Green;
		
		const static vec4f Cyan;
		const static vec4f Yellow;
		const static vec4f Magenta;

		const static vec4f Violet;
		const static vec4f Lime;

		const static vec4f Orange;
		const static vec4f Fuchsia;

		const static vec4f Aqua;
		const static vec4f Azure;
	};
}

#endif // !_COLOR_H_
