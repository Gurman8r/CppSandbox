#include <MemeAudio/AudioBuffer.hpp>
#include <MemeAudio/OpenAL.hpp>

namespace ml
{
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

	AudioBuffer & AudioBuffer::clean()
	{
		if ((*this))
		{
			OpenAL::deleteBuffers(1, (*this));
		}
		return (*this);
	}

	AudioBuffer & AudioBuffer::create()
	{
		if (!(*this) && (get_ref() = OpenAL::genBuffers(1)))
		{

		}
		return (*this);
	}
}