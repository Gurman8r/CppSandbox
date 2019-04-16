#ifndef _ML_ICON_HPP_
#define _ML_ICON_HPP_

#include <MemeWindow/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_WINDOW_API Icon final
		: public ITrackable
	{
		uint32_t		width;
		uint32_t		height;
		const uint8_t *	pixels;

		/* * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * */

		inline void serialize(std::ostream & out) const override
		{
			out << GetTypeInfo().name() << " " << width << " " << height << " ";
		}

		inline void deserialize(std::istream & in) override
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ICON_HPP_