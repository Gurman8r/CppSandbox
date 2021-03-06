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
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MATHS_HPP_