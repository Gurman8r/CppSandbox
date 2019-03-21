#ifndef _ML_AL_HPP_
#define _ML_AL_HPP_

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

#endif // !_ML_AL_HPP_