#include <MemeAudio/Audio.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
		OpenAL::closeDevice();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}