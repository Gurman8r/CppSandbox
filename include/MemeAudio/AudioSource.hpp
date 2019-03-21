#ifndef _ML_AUDIO_SOURCE_HPP_
#define _ML_AUDIO_SOURCE_HPP_

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
		AudioSource & create(uint32_t count);

	private:
		uint32_t m_count;
	};
}

#endif // !_ML_AUDIO_SOURCE_HPP_