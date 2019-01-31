#include <MemeAudio/Audio.h>
#include <MemeCore/Debug.h>

namespace ml
{
	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
		OpenAL::closeDevice();
	}

}