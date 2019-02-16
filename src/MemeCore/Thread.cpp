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
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Thread & Thread::clean()
	{
		wait();

		if (m_thr) { delete m_thr; }
		
		if (m_fun) { delete m_fun; }

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

	Thread & Thread::update(Function * fun)
	{
		if (m_fun != fun)
		{
			clean();

			m_fun = fun;
		}
		return (*this);
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
		return (m_fun ? (*m_fun)() : void());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}