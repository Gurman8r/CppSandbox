#include <MemeAudio/AudioSource.h>
#include <MemeAudio/OpenAL.h>

namespace ml
{
	AudioSource::AudioSource()
		: IHandle(NULL)
		, m_count(0)
		, m_size(0)
	{
	}
	AudioSource::AudioSource(const AudioSource & copy)
		: IHandle(copy)
		,m_count(copy.m_count)
		,m_size(copy.m_size)
	{
	}

	AudioSource & AudioSource::generate(size_t count, size_t size)
	{
		if (!(*this) && (count && size))
		{
			m_count = count;
			m_size = size;
			OpenAL::genSources(count, size, (*this));
		}
		return (*this);
	}
}