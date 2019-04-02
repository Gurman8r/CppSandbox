#ifndef _ML_TIME_HPP_
#define _ML_TIME_HPP_

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

		float sin() const;

		float cos() const;

		uint64_t calculateFPS(float deltaTime);

		void sleep(const ml::Duration & value);
	
	private:
		Time() {}
		~Time() {}

		Timer		m_mainTimer;
		uint64_t	m_frameCounter = 0;
		uint64_t	m_fps = 0;
		float		m_nextSecond = 0.0f;
		float		m_prevSecond = 0.0f;
	};
}

#endif // !_ML_TIME_HPP_