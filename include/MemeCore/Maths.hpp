#ifndef _ML_MATHS_HPP_
#define _ML_MATHS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>
#include <MemeCore/StandardLib.hpp>
#include <MemeCore/Preprocessor.hpp>
#include <MemeCore/GLM.hpp>

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
		> inline static T clamp(const T & value, const T & min, const T & max)
		{
			return static_cast<T>(ML_CLAMP(
				static_cast<T>(value),
				static_cast<T>(min),
				static_cast<T>(max)
			));
		}

		template <
			class T
		> inline static T lerp(const T & a, const T & b, const float t)
		{
			return static_cast<T>(ML_LERP(
				static_cast<T>(a),
				static_cast<T>(b),
				static_cast<T>(t)
			));
		}

		template <
			class T
		> inline static T mapRange(const T & value, const T & min1, const T & max1, const T & min2, const T & max2)
		{
			return static_cast<T>(ML_MAP_RANGE(
				static_cast<T>(value), 
				static_cast<T>(min1),
				static_cast<T>(max1), 
				static_cast<T>(min2), 
				static_cast<T>(max2)
			));
		}

		template <
			class T
		> inline static T sign(const T & value)
		{
			return static_cast<T>(ML_SIGN(
				static_cast<T>(value)
			));
		}

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATHS_HPP_