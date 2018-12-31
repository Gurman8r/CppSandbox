#ifndef _MATHS_H_
#define _MATHS_H_

#include <MemeLib/Export.h>

namespace ml
{
	class ML_API Maths
	{
	public:
		static const float Pi;
		static const float Deg2Rad;
		static const float Rad2Deg;

	public:
		inline static float toRadians(float degrees)
		{
			return degrees * Deg2Rad;
		}

		inline static float toDegrees(float radians)
		{
			return radians * Rad2Deg;
		}


	public:
		template <typename T>
		inline static const T clamp(T value, T min, T max)
		{
			return (value < min)
				? (min)
				: (value > max)
					? (max)
					: (value);
		}

		template <typename T>
		inline static T mapRange(T value, T min1, T max1, T min2, T max2)
		{
			return (min2 + (value - min1) * (max2 - min2) / (max1 - min1));
		}

		template <typename T>
		inline static const T sign(const T & value)
		{
			return (value == (T)0)
				? ((T)0)
				: (value > (T)0)
					? ((T)1)
					: ((T)-1);
		}

	};
}

#endif // !_MATHS_H_
