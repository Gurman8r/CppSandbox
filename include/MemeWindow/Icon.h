#ifndef _ICON_H_
#define _ICON_H_

#include <MemeWindow/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	struct ML_WINDOW_API Icon final
		: public ITrackable
	{
		uint32_t		width;
		uint32_t		height;
		const uint8_t *	pixels;

		Icon(uint32_t width, uint32_t height, const uint8_t * pixels)
			: width(width)
			, height(height)
			, pixels(pixels)
		{
		}
		Icon(const Icon & copy)
			: width(copy.width)
			, height(copy.height)
			, pixels(copy.pixels)
		{
		}
	};
}

#endif // !_ICON_H_
