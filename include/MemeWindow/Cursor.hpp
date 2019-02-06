#ifndef _CURSOR_HPP_
#define _CURSOR_HPP_

#include <MemeWindow/Export.hpp>

namespace ml
{
	struct ML_WINDOW_API Cursor
	{
		enum Mode
		{
			Normal = 0x34001,
			Hidden,
			Disabled,
		};

		enum Standard
		{
			Arrow = 0x36001, 
			IBeam, 
			Crosshair,
			Hand,
			HResize,
			VResize,
		};
	};
}

#endif // !_CURSOR_HPP_
