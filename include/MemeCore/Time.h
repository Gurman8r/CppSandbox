#ifndef _TIME_H_
#define _TIME_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/Timer.h>

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
	
	private:
		Time() { }
		~Time() {}

		Timer m_timer;
	};
}

#endif // !_TIME_H_
