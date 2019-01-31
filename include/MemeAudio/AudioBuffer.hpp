#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_AUDIO_API AudioBuffer
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		AudioBuffer();
		AudioBuffer(const AudioBuffer & copy);
		~AudioBuffer();

		AudioBuffer & clean();
		AudioBuffer & create();

	private:
		size_t m_count;
	};
}

#endif // !_AUDIO_BUFFER_H_
