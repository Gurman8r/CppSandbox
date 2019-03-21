#ifndef _ML_MATHS_HPP_
#define _ML_MATHS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>
#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_CLAMP(value, min, max) \
(((value) > (max)) \
	? (((value) < (min)) \
		? (min) \
		: (value)) \
	: (max)) \

#define ML_TYPE_CLAMP(T, value, min, max) \
	static_cast<T>(ML_CLAMP(value, min, max)) \

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
		template <typename T>
		inline static const T clamp(T value, T min, T max)
		{
			return ML_TYPE_CLAMP(T, value, min, max);
		}

		template <typename T>
		inline static const T lerp(T a, T b, float t)
		{
			return static_cast<T>(a * t + b * (1 - t));
		}

		template <typename T>
		inline static T mapRange(T value, T min1, T max1, T min2, T max2)
		{
			return static_cast<T>(min2 + (value - min1) * (max2 - min2) / (max1 - min1));
		}

		template <typename T>
		inline static const T sign(const T & value)
		{
			return static_cast<T>(
				(((value) == (0))
					? (0)
					: (((value) > (0))
						? (+1)
						: (-1))));
		}

	};
}

#endif // !_ML_MATHS_HPP_