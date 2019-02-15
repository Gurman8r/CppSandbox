#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/Timer.hpp>

#define ML_Time ml::Time::getInstance()

namespace ml
{
	class ML_CORE_API Time final
		: public ITrackable
		, public ISingleton<Time>
	{
		friend class ISingleton<Time>;

	public:
		void start();
		
		const Timer & timer() const;

		const Duration & elapsed() const;

		static uint64_t calculateFPS(float deltaTime);

		static void sleep(const ml::Duration & value);
	
	private:
		Time() { }
		~Time() {}

		Timer m_timer;
	};
}

#endif // !_TIME_HPP_
