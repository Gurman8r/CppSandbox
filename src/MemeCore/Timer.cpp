#include <MemeCore/Timer.h>

namespace ml
{
	Timer::Timer()
		: m_paused(true)
		, m_elapsed(0)
		, m_clock()
		, m_min()
		, m_max()
	{
	}

	Timer::Timer(const Timer & copy)
		: m_paused(copy.m_paused)
		, m_elapsed(copy.m_elapsed)
		, m_clock(copy.m_clock)
		, m_min(copy.m_min)
		, m_max(copy.m_max)
	{
	}

	Timer::~Timer()
	{
	}


	Timer& Timer::pause(bool pause)
	{
		if (pause && !m_paused)
		{
			m_paused = true;
			m_max = m_clock.now();
			m_elapsed = (m_max - m_min);
		}
		else if (!pause && m_paused)
		{
			m_paused = false;
			m_min = m_clock.now();
		}
		return (*this);
	}

	Timer& Timer::reset()
	{
		return stop().start();
	}

	Timer& Timer::start()
	{
		m_min = m_max = m_clock.now();
		m_elapsed = TimePoint();
		return pause(false);
	}

	Timer& Timer::stop()
	{
		m_max = m_clock.now();
		m_elapsed = (m_max - m_min);
		return (*this);
	}


	const TimePoint & Timer::elapsed() const
	{
		return (m_paused)
			? m_elapsed
			: m_elapsed = (m_clock.now() - m_min);
	}

	const bool & Timer::paused() const
	{
		return m_paused;
	}
}
