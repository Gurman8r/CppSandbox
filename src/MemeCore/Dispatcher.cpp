#include <MemeCore/Dispatcher.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Dispatcher::Dispatcher()
	{
	}

	Dispatcher::~Dispatcher()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Dispatcher::init(size_t value)
	{
		std::thread * t = NULL;
		Worker * w = NULL;

		for (size_t i = 0; i < value; i++)
		{
			w = new Worker();
			
			m_allWorkers.push_back(w);

			t = new std::thread(&Worker::run, w);

			m_threads.push_back(t);
		}

		return (bool)(value);
	}

	bool Dispatcher::stop()
	{
		for (Worker * w : m_allWorkers)
		{
			w->stop();
		}
		Debug::log("Stopped Workers");

		for (std::thread * t : m_threads)
		{
			t->join();
		}
		Debug::log("Joined Threads");

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Dispatcher::addRequest(IRequest * value)
	{
		m_mutex_work.lock();

		if (!m_workers.empty())
		{
			Worker * w = m_workers.front();
			w->setRequest(value);
			Worker::Condition * cv;
			w->getCondition(cv);
			cv->notify_one();
			m_workers.pop();
			m_mutex_work.unlock();
		}
		else
		{
			m_mutex_work.unlock();
			m_mutex_req.lock();
			m_requests.push(value);
			m_mutex_req.unlock();
		}
	}

	bool Dispatcher::addWorker(Worker * value)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}