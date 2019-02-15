#include <MemeCore/Thread.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

#define ML_HANDLE(value) reinterpret_cast<HANDLE>(value)

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Thread::Thread()
		: m_fun(NULL)
		, m_thread(NULL)
	{
	}

	Thread::~Thread()
	{
		join();
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Thread & Thread::cleanup()
	{
		if (m_fun)
		{
			delete m_fun;
			m_fun = NULL;
		}
		if (m_thread)
		{
			delete m_thread;
			m_thread = NULL;
		}
		return (*this);
	}

	Thread & Thread::detatch()
	{
		if (m_thread)
		{
			m_thread->detach();
		}
		return (*this);
	}

	Thread & Thread::launch()
	{
		if (!m_thread)
		{
			m_thread = new std::thread(&Thread::run, this);
		}
		return (*this);
	}

	Thread & Thread::join()
	{
		if (m_thread)
		{
			m_thread->join();
		}
		return (*this);
	}

	Thread & Thread::swap(Thread & other)
	{
		std::swap(m_thread, other.m_thread);
		std::swap(m_fun,	other.m_fun);
		return other;
	}

	Thread & Thread::terminate()
	{
		if (m_thread)
		{

		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Thread::run()
	{
		if (m_fun)
		{
			return m_fun->run();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}