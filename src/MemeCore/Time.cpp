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

	uint64_t Time::calcStableFPS(float deltaTime)
	{
		static uint64_t	frameCounter = 0;
		static uint64_t	fps = 0;
		static float	nextSecond = 0.0f;
		static float	prevSecond = 0.0f;
		frameCounter++;
		if (((nextSecond += deltaTime) - prevSecond) > 1.0f)
		{
			prevSecond = nextSecond;
			fps = frameCounter;
			frameCounter = 0;
		}
		return fps;
	}
}