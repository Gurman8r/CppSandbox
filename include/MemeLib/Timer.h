#ifndef _TIMER_H_
#define _TIMER_H_

#include <MemeLib/TimePoint.h>

namespace ml
{
	class ML_API Timer final
		: public ITrackable
	{
	public:
		using clock_type = std::chrono::high_resolution_clock;
		using point_type = std::chrono::steady_clock::time_point;

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

		bool		m_paused;
		clock_type	m_clock;
		point_type	m_min;
		point_type	m_max;
	};
}

#endif // !_TIMER_H_
