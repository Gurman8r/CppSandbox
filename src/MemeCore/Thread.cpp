#include <MemeCore/Thread.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Thread::Thread()
		: m_thr(NULL)
	{
	}

	Thread::~Thread()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Thread::alive() const
	{
		return (bool)(m_thr);
	}

	bool Thread::joinable() const
	{
		return alive() && m_thr->joinable();
	}

	bool Thread::dispose()
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

	void Thread::sleep(const Duration & value)
	{
		if (joinable())
		{
			std::this_thread::sleep_for(value.base());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}