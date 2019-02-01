#ifndef _ICON_HPP_
#define _ICON_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_WINDOW_API Icon final
		: public ITrackable
	{
	public:
		uint32_t		width;
		uint32_t		height;
		const uint8_t *	pixels;

		Icon()
			: width(0)
			, height(0)
			, pixels(0)
		{
		}
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

		inline void serialize(std::ostream & out) const override
		{
			out << get_type().name() << ' ' << width << ' ' << height << ' ';
		}
	};
}

#endif // !_ICON_HPP_
