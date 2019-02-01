#ifndef _AL_H_
#define _AL_H_

#include <MemeAudio/Export.hpp>
#include <MemeCore/Maths.hpp>

namespace ml
{
	struct ML_AUDIO_API AL final
	{
		using Enum = int32_t;
		using Size = int32_t;

		enum Err
		{
			NoError,
			InvalidName,
			InvalidEnum,
			InvalidValue,
			InvalidOperation,
			OutOfMemory,

			MAX_ERROR
		};
	};
}

#endif // !_AL_H_
