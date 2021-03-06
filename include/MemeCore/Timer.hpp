#ifndef _ML_TIMER_HPP_
#define _ML_TIMER_HPP_

#include <MemeCore/Duration.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Timer final
		: public ITrackable
	{
	public:
		using Clock		= std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::steady_clock::time_point;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Timer();
		Timer(const Timer & copy);
		~Timer();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Timer & pause(bool pause);
		Timer & reset();
		Timer & start();
		Timer & stop();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		const Duration & elapsed() const;
		const bool & paused() const;

	private:
		bool		m_paused;
		Clock		m_clock;
		TimePoint	m_prev;
		TimePoint	m_next;

		mutable Duration m_elapsed;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TIMER_HPP_