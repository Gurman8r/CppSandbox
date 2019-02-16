#include <MemeAudio/Sound.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Sound::Sound()
	{
		
	}

	Sound::~Sound()
	{
		cleanup();
	}

	bool Sound::cleanup()
	{
		return false;
	}

	bool Sound::readFile(const String & filename)
	{
		return false;
	}
}