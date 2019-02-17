#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Vector4.hpp>

namespace ml
{
	class ML_GRAPHICS_API Color
		: public ITrackable
	{
	public:
		const static vec4f Clear;
		const static vec4f White;
		const static vec4f Black;
		const static vec4f Gray;

		const static vec4f Red;
		const static vec4f Blue;
		const static vec4f Green;
		
		const static vec4f Cyan;
		const static vec4f Yellow;
		const static vec4f LightYellow;
		const static vec4f Magenta;

		const static vec4f Violet;
		const static vec4f Lime;

		const static vec4f Orange;
		const static vec4f Fuchsia;

		const static vec4f Aqua;
		const static vec4f Azure;
	};
}

#endif // !_COLOR_HPP_
