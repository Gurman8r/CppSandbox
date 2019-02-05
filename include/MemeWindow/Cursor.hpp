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
			Hidden = 0x34002,
			Disabled = 0x34003,
		};
	};
}

#endif // !_CURSOR_HPP_
