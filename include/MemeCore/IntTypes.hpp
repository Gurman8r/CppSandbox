#ifndef _ML_INT_TYPES_H_
#define _ML_INT_TYPES_H_

/* * * * * * * * * * * * * * * * * * * * */

#include <stdint.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
#ifdef ML_INT_TYPES
	using int8_t   = signed char;
	using int16_t  = short;
	using int32_t  = int;
	using int64_t  = long long;
	using uint8_t  = unsigned char;
	using uint16_t = unsigned short;
	using uint32_t = unsigned int;
	using uint64_t = unsigned long long;
#endif // ML_INT_TYPES
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	enum ML_Enum : uint32_t
	{
		ML_ENUM_0	= (0 << 0), // |  0  | 0x000 | (0 << 0)
		ML_ENUM_1	= (1 << 0), // |  1  | 0x001 | (1 << 0)
		ML_ENUM_2	= (1 << 1), // |  2  | 0x002 | (1 << 1)
		ML_ENUM_4	= (1 << 2), // |  4  | 0x004 | (1 << 2)
		ML_ENUM_8	= (1 << 3), // |  8  | 0x008 | (1 << 3)
		ML_ENUM_16	= (1 << 4), // |  16 | 0x010 | (1 << 4)
		ML_ENUM_32	= (1 << 5), // |  32 | 0x020 | (1 << 5)
		ML_ENUM_64	= (1 << 6), // |  64 | 0x040 | (1 << 6)
		ML_ENUM_128 = (1 << 7), // | 128 | 0x080 | (1 << 7)
		ML_ENUM_256	= (1 << 8), // | 256 | 0x100 | (1 << 8)
		
		ML_MAX_ENUM = 10
	};

	const ML_Enum ML_EnumValues[ML_Enum::ML_MAX_ENUM] = {
		ML_ENUM_0,
		ML_ENUM_1,
		ML_ENUM_2,
		ML_ENUM_4,
		ML_ENUM_8,
		ML_ENUM_16,
		ML_ENUM_32,
		ML_ENUM_64,
		ML_ENUM_128,
		ML_ENUM_256,
	};

	/* * * * * * * * * * * * * * * * * * * * */

	enum ML_Flag : uint32_t
	{
		ML_FLAG_0 = ML_ENUM_0,	// |  0  | 0x000 | (0 << 0)
		ML_FLAG_1 = ML_ENUM_1,	// |  1  | 0x001 | (1 << 0)
		ML_FLAG_2 = ML_ENUM_2,	// |  2  | 0x002 | (1 << 1)
		ML_FLAG_3 = ML_ENUM_4,	// |  4  | 0x004 | (1 << 2)
		ML_FLAG_4 = ML_ENUM_8,	// |  8  | 0x008 | (1 << 3)
		ML_FLAG_5 = ML_ENUM_16,	// |  16 | 0x010 | (1 << 4)
		ML_FLAG_6 = ML_ENUM_32,	// |  32 | 0x020 | (1 << 5)
		ML_FLAG_7 = ML_ENUM_64,	// |  64 | 0x040 | (1 << 6)
		ML_FLAG_8 = ML_ENUM_128,// | 128 | 0x080 | (1 << 7)
		ML_FLAG_9 = ML_ENUM_256,// | 256 | 0x100 | (1 << 8)

		ML_MAX_FLAG = 10
	};

	const ML_Flag ML_FlagValues[ML_Flag::ML_MAX_FLAG] = {
		ML_FLAG_0,
		ML_FLAG_1,
		ML_FLAG_2,
		ML_FLAG_3,
		ML_FLAG_4,
		ML_FLAG_5,
		ML_FLAG_6,
		ML_FLAG_7,
		ML_FLAG_8,
		ML_FLAG_9,
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_INT_TYPES_H_