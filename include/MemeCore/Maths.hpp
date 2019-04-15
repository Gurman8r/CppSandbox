#ifndef _ML_MATHS_HPP_
#define _ML_MATHS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>
#include <MemeCore/STD.hpp>
#include <MemeCore/Preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_DEG2RAD(degrees) (ml::Maths::Deg2Rad * degrees)
#define ML_RAD2DEG(radians) (ml::Maths::Rad2Deg * radians)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Maths final
	{
	public: // Constants
		/* * * * * * * * * * * * * * * * * * * * */
		static const float Pi;
		static const float Deg2Rad;
		static const float Rad2Deg;

	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
		template <
			class T
		> inline static const T clamp(T value, T min, T max)
		{
			return static_cast<T>(ML_CLAMP(value, min, max));
		}

		template <
			class T
		> inline static const T lerp(T a, T b, float t)
		{
			return static_cast<T>(ML_LERP(a, b, t));
		}

		template <
			class T
		> inline static T mapRange(T value, T min1, T max1, T min2, T max2)
		{
			return static_cast<T>(ML_MAP_RANGE(value, min1, max1, min2, max2));
		}

		template <
			class T
		> inline static const T sign(const T & value)
		{
			return static_cast<T>(ML_SIGN(value));
		}

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATHS_HPP_