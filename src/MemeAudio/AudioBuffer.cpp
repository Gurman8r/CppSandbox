#include <MemeAudio/AudioBuffer.hpp>
#include <MemeAudio/OpenAL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	AudioBuffer::AudioBuffer()
		: IHandle(NULL)
		, m_count(0)
	{
	}
	AudioBuffer::AudioBuffer(const AudioBuffer & copy)
		: IHandle(copy)
		, m_count(copy.m_count)
	{
	}

	AudioBuffer::~AudioBuffer()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AudioBuffer & AudioBuffer::clean()
	{
		if ((*this))
		{
			OpenAL::deleteBuffers(m_count, (*this));
		}
		return (*this);
	}

	AudioBuffer & AudioBuffer::create(uint32_t count)
	{
		if (set_handle(ML_AL.genBuffers(count)))
		{
			m_count = count;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}