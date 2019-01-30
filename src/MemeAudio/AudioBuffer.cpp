#include <MemeAudio/AudioBuffer.h>
#include <MemeAudio/OpenAL.h>

namespace ml
{
	AudioBuffer::AudioBuffer()
		: IHandle(NULL)
		, m_count(0)
		, m_size(0)
	{
	}
	AudioBuffer::AudioBuffer(const AudioBuffer & copy)
		: IHandle(copy)
		, m_count(copy.m_count)
		, m_size(copy.m_size)
	{
	}

	AudioBuffer::~AudioBuffer()
	{
	}

	AudioBuffer & AudioBuffer::generate(size_t count, size_t size)
	{
		if (!(*this) && (count && size))
		{
			m_count = count;
			m_size = size;
			OpenAL::genBuffers(count, size, (*this));
		}
		return (*this);
	}
}