#include <MemeCore/Time.h>

namespace ml
{
	const Timer & Time::timer() const
	{
		return m_timer;
	}

	const Duration & Time::elapsed() const
	{
		return timer().elapsed();
	}

	void Time::start()
	{
		m_timer.start();
	}
}