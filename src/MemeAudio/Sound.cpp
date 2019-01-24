#include <MemeAudio/Sound.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace ml
{
	Sound::Sound()
	{
		m_device = static_cast<ALCdevice*>(alcOpenDevice(NULL));
	}

	Sound::~Sound()
	{
		alcCloseDevice(static_cast<ALCdevice*>(m_device));
	}

	bool Sound::cleanup()
	{
		return false;
	}

	bool Sound::loadFromFile(const std::string & filename)
	{
		return false;
	}
}