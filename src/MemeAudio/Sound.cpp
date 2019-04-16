#include <MemeAudio/Sound.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sound::Sound()
	{
		
	}

	Sound::~Sound()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sound::dispose()
	{
		return false;
	}

	bool Sound::loadFromFile(const String & filename)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}