#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <MemeWindow/Export.h>
#include <MemeCore/Vector2.h>

namespace ml
{
	class ML_WINDOW_API Screen final
	{
	public:
		static vec2u  size();
	};
}

#endif // !_SCREEN_H_
