#ifndef _ML_USINGS_HPP_
#define _ML_USINGS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>
#include <stdint.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
//#define ML_INT_TYPES
# ifdef ML_INT_TYPES
	using int8_t   = signed char;
	using int16_t  = short;
	using int32_t  = int;
	using int64_t  = long long;

	using uint8_t  = unsigned char;
	using uint16_t = unsigned short;
	using uint32_t = unsigned int;
	using uint64_t = unsigned long long;

#	ifdef ML_x64
	using size_t	= uint64_t;
	using ptrdiff_t = int64_t;
	using intptr_t	= int64_t;
#	else
	using size_t	= uint32_t;
	using ptrdiff_t = int32_t;
	using intptr_t	= int32_t;
#	endif
# endif
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_USINGS_HPP_