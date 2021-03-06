#ifndef _ML_AUDIO_BUFFER_HPP_
#define _ML_AUDIO_BUFFER_HPP_

#include <MemeAudio/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_AUDIO_API AudioBuffer
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		AudioBuffer();
		AudioBuffer(const AudioBuffer & copy);
		~AudioBuffer();

	public:
		AudioBuffer & clean();
		AudioBuffer & create(uint32_t count);

	private:
		uint32_t m_count;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AUDIO_BUFFER_HPP_