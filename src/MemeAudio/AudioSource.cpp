#include <MemeAudio/AudioSource.hpp>
#include <MemeAudio/OpenAL.hpp>

namespace ml
{
	AudioSource::AudioSource()
		: IHandle(NULL)
		, m_count(0)
	{
	}
	AudioSource::AudioSource(const AudioSource & copy)
		: IHandle(copy)
		, m_count(copy.m_count)
	{
	}
	AudioSource::~AudioSource()
	{
		clean();
	}

	AudioSource & AudioSource::clean()
	{
		if ((*this))
		{
			OpenAL::deleteBuffers(m_count, (*this));
		}
		return (*this);
	}

	AudioSource & AudioSource::create(uint32_t count)
	{
		if (!(*this) && (get_ref() = OpenAL::genSources(count)))
		{
			m_count = count;
		}
		return (*this);
	}
}