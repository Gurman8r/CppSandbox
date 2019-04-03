#include <MemeCore/Thread.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Thread::Thread()
		: m_thr(NULL)
		, m_lock()
	{
	}

	Thread::~Thread()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	inline bool Thread::alive() const
	{
		return (bool)(m_thr);
	}

	inline bool Thread::joinable() const
	{
		return alive() && m_thr->joinable();
	}

	inline bool Thread::cleanup()
	{
		if (joinable())
		{
			m_thr->join();
		}
		if (alive())
		{
			delete m_thr;
			m_thr = NULL;
		}
		return !(m_thr);
	}

	inline void Thread::sleep(const ml::Duration & value)
	{
		if (joinable())
		{
			std::this_thread::sleep_for((Millis)value);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}