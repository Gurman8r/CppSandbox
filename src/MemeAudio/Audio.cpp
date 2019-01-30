#include <MemeAudio/Audio.h>
#include <MemeAudio/OpenAL.h>
#include <MemeCore/Debug.h>

#define DEVICE(ptr) static_cast<ALCdevice *>(ptr)
#define CONTEXT(ptr) static_cast<ALCcontext *>(ptr)

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