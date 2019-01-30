#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include <MemeCore/ISingleton.h>
#include <MemeAudio/Sound.h>
#include <MemeAudio/AudioBuffer.h>
#include <MemeAudio/AudioSource.h>

#define ML_Audio ml::Audio::getInstance()

namespace ml
{
	class ML_AUDIO_API Audio
		: public ITrackable
		, public ISingleton<Audio>
	{
		friend class ISingleton<Audio>;

	public:

	private:
		Audio();
		~Audio();
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_AUDIO_MANAGER_H_
