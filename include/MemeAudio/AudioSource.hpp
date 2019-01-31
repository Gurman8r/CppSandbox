#ifndef _AUDIO_SOURCE_H_
#define _AUDIO_SOURCE_H_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_AUDIO_API AudioSource
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		AudioSource();
		AudioSource(const AudioSource & copy);
		~AudioSource();

		AudioSource & clean();
		AudioSource & create();

	private:
		size_t m_count;
	};
}

#endif // !_AUDIO_SOURCE_H_
