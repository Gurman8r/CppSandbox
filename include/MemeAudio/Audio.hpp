#ifndef _ML_AUDIO_HPP_
#define _ML_AUDIO_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeAudio/OpenAL.hpp>
#include <MemeAudio/Sound.hpp>
#include <MemeAudio/AudioBuffer.hpp>
#include <MemeAudio/AudioSource.hpp>

#define ML_Audio ml::Audio::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API Audio
		: public ITrackable
		, public ISingleton<Audio>
	{
		friend class ISingleton<Audio>;

	private:
		Audio();
		~Audio();

	public:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_HPP_