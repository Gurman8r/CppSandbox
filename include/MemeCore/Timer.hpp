#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <MemeCore/Duration.hpp>

namespace ml
{
	class ML_CORE_API Timer final
		: public ITrackable
	{
	public:
		using Clock		= std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::steady_clock::time_point;

	public:
		Timer();
		Timer(const Timer& copy);
		~Timer();

		Timer & pause(bool pause);
		Timer & reset();
		Timer & start();
		Timer & stop();

		const Duration & elapsed() const;
		const bool & paused() const;

	private:
		mutable Duration m_elapsed;

		bool		m_paused;
		Clock		m_clock;
		TimePoint	m_prev;
		TimePoint	m_next;
	};
}

#endif // !_TIMER_HPP_
