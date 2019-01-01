#ifndef _ENUM_H_
#define _ENUM_H_

#include <inttypes.h>

namespace ml
{
	enum ML_Enum : uint16_t
	{
		ML_ENUM_0	= (0 << 0), // [0] |  0  | 0x0000 | (0 << 0)
		ML_ENUM_1	= (1 << 0), // [1] |  1  | 0x0001 | (1 << 0)
		ML_ENUM_2	= (1 << 1), // [2] |  2  | 0x0002 | (1 << 1)
		ML_ENUM_4	= (1 << 2), // [3] |  4  | 0x0004 | (1 << 2)
		ML_ENUM_8	= (1 << 3), // [4] |  8  | 0x0008 | (1 << 3)
		ML_ENUM_16	= (1 << 4), // [5] |  16 | 0x0010 | (1 << 4)
		ML_ENUM_32	= (1 << 5), // [6] |  32 | 0x0020 | (1 << 5)
		ML_ENUM_64	= (1 << 6), // [7] |  64 | 0x0040 | (1 << 6)
		ML_ENUM_128 = (1 << 7), // [8] | 128 | 0x0080 | (1 << 7)
		ML_ENUM_256	= (1 << 8), // [9] | 256 | 0x0100 | (1 << 8)

		ML_FLAG_0 = ML_ENUM_0,	// ML_ENUM_0   | [0] |  0  | 0x0000 | (0 << 0)
		ML_FLAG_1 = ML_ENUM_1,	// ML_ENUM_1   | [1] |  1  | 0x0001 | (1 << 0)
		ML_FLAG_2 = ML_ENUM_2,	// ML_ENUM_2   | [2] |  2  | 0x0002 | (1 << 1)
		ML_FLAG_3 = ML_ENUM_4,	// ML_ENUM_4   | [3] |  4  | 0x0004 | (1 << 2)
		ML_FLAG_4 = ML_ENUM_8,	// ML_ENUM_8   | [4] |  8  | 0x0008 | (1 << 3)
		ML_FLAG_5 = ML_ENUM_16,	// ML_ENUM_16  | [5] |  16 | 0x0010 | (1 << 4)
		ML_FLAG_6 = ML_ENUM_32,	// ML_ENUM_32  | [6] |  32 | 0x0020 | (1 << 5)
		ML_FLAG_7 = ML_ENUM_64,	// ML_ENUM_64  | [7] |  64 | 0x0040 | (1 << 6)
		ML_FLAG_8 = ML_ENUM_128,// ML_ENUM_128 | [8] | 128 | 0x0080 | (1 << 7)
		ML_FLAG_9 = ML_ENUM_256,// ML_ENUM_256 | [9] | 256 | 0x0100 | (1 << 8)
		
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
}

#endif // !_ENUM_H_
