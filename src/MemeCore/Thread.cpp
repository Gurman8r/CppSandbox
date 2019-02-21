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
		assert(!m_thr);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Thread & Thread::clean()
	{
		join();

		return update(NULL);
	}

	Thread & Thread::detatch()
	{
		if (alive())
		{
			m_thr->detach();
		}
		return (*this);
	}

	Thread & Thread::join()
	{
		if (joinable())
		{
			m_thr->join();
			delete m_thr;
			m_thr = NULL;
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
			if (m_fun)
			{ 
				delete m_fun;
			}
			m_fun = fun;
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