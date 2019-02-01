#ifndef _MATHS_HPP_
#define _MATHS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>
#include <MemeCore/IntTypes.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_CLAMP(value, min, max) \
(((value) > (max)) \
	? (((value) < (min)) \
		? (min) \
		: (value)) \
	: (max)) \

#define ML_TYPE_CLAMP(type, value, min, max) \
static_cast<type>(ML_CLAMP(value, min, max)) \

#define ML_MAP(value, min1, max1, min2, max2) \
(min2 + (value - min1) * (max2 - min2) / (max1 - min1)) \

#define ML_SIGN(value) \
(((value) == (0)) \
	? (0) \
	: (((value) > (0)) \
		? (+1) \
		: (-1))) \

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	class ML_CORE_API Maths
	{
	public:
		static const float Pi;
		static const float Deg2Rad;
		static const float Rad2Deg;

	public:
		template <class T>
		inline static const T clamp(T value, T min, T max)
		{
			return static_cast<T>(ML_CLAMP(value, min, max));
		}

		template <class T>
		inline static T mapRange(T value, T min1, T max1, T min2, T max2)
		{
			return static_cast<T>(ML_MAP(value, min1, max1, min2, max2));
		}

		template <class T>
		inline static const T sign(const T & value)
		{
			return static_cast<T>(ML_SIGN(value));
		}

	};
}

#endif // !_MATHS_HPP_
