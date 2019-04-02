#include <MemePhysics/PhysicsThread.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsThread::PhysicsThread()
		: m_thread(NULL)
	{
	}

	PhysicsThread::~PhysicsThread()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	std::thread * PhysicsThread::launch(std::thread * value)
	{
		return ((!m_thread) ? (m_thread = value) : (NULL));
	}

	bool PhysicsThread::cleanup()
	{
		if (wait() || (m_thread))
		{
			delete m_thread;
			m_thread = NULL;
		}
		return !(m_thread);
	}

	bool PhysicsThread::wait()
	{
		bool good;
		if (good = (m_thread && m_thread->joinable()))
		{
			m_thread->join();
		}
		return good;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}