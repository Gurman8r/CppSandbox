#include <MemeAudio/Sound.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>

namespace ml
{
	Sound::Sound()
	{
		m_device = static_cast<ALCdevice*>(alcOpenDevice(NULL));
	}

	Sound::~Sound()
	{
	}
}