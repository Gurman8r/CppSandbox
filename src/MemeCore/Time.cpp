#include <MemeCore/Time.hpp>

namespace ml
{
	void Time::start()
	{
		m_mainTimer.start();
	}

	const Timer & Time::timer() const
	{
		return m_mainTimer;
	}

	const Duration & Time::elapsed() const
	{
		return timer().elapsed();
	}

	float Time::sin() const
	{
		return sinf(elapsed().delta());
	}

	float Time::cos() const
	{
		return cosf(elapsed().delta());
	}

	uint64_t Time::calculateFPS(float deltaTime)
	{
		m_frameCounter++;
		if (((m_nextSecond += deltaTime) - m_prevSecond) > 1.0f)
		{
			m_prevSecond = m_nextSecond;
			m_fps = m_frameCounter;
			m_frameCounter = 0;
		}
		return m_fps;
	}

	void Time::sleep(const ml::Duration & value)
	{
		static Timer t;
		t.reset();
		while (t.elapsed() < value);
	}
}