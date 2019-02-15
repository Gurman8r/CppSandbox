#include <MemeCore/Thread.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Thread::Thread()
		: m_fun(NULL)
		, m_thr(NULL)
	{
	}

	Thread::~Thread()
	{
		wait();
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
		if (m_thr)
		{
			delete m_thr;
			m_thr = NULL;
		}
		return (*this);
	}

	Thread & Thread::detatch()
	{
		if (alive())
		{
			m_thr->detach();
		}
		return (*this);
	}

	Thread & Thread::launch()
	{
		if (!alive())
		{
			m_thr = new std::thread(&Thread::run, this);
		}
		return (*this);
	}

	Thread & Thread::swap(Thread & other)
	{
		std::swap(m_fun, other.m_fun);
		std::swap(m_thr, other.m_thr);
		return other;
	}

	Thread & Thread::wait()
	{
		if (joinable())
		{
			m_thr->join();
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Thread::alive() const
	{
		return m_thr;
	}

	void * Thread::handle()
	{
		return (alive() ? m_thr->native_handle() : NULL);
	}

	Thread::ID Thread::id() const
	{
		return (alive() ? m_thr->get_id() : ID());
	}

	bool Thread::joinable() const
	{
		return (alive() && m_thr->joinable());
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Thread::run()
	{
		return (m_fun ? m_fun->run() : void());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}