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

		template <typename T>
		inline static const T sign(const T & value)
		{
			return (value == (T)0)
				? (T)0
				: (value > (T)0)
					? (T)1
					: (T)-1;
		}

	};
}

#endif // !_MATHS_H_
