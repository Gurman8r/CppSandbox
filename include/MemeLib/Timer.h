#ifndef _TIMER_H_
#define _TIMER_H_

#include <MemeLib/TimePoint.h>

namespace ml
{
	class ML_API Timer final
		: public ITrackable
	{
	public:
		Timer();
		Timer(const Timer& copy);
		~Timer();

		Timer& pause(bool pause);
		Timer& reset();
		Timer& start();
		Timer& stop();

		TimePoint	elapsed() const;
		bool		paused() const;

	private:
		mutable TimePoint m_elapsed;
		bool m_paused;

		std::chrono::high_resolution_clock		m_clock;
		std::chrono::steady_clock::time_point	m_min;
		std::chrono::steady_clock::time_point	m_max;
	};
}

#endif // !_TIMER_H_
