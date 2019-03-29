#ifndef _TESTING_HPP_
#define _TESTING_HPP_

#include <MemeCore/Debug.hpp>
#include <MemeCore/Time.hpp>
#include <MemeCore/Dispatcher.hpp>
#include <MemeCore/TestRequest.hpp>

// https://www.youtube.com/watch?v=hWQN2XW6WUM

static std::sig_atomic_t signal_caught = 0;
static std::mutex logMutex;

inline static void logFunc(const ml::String & value)
{
	logMutex.lock();
	ml::cout << value << ml::endl;
	logMutex.unlock();
}

inline static void sigint_handler(int32_t value)
{
	signal_caught = 1;
}

inline static int32_t testDispatcher()
{
	std::signal(SIGINT, &sigint_handler);
	ML_Dispatcher.init(10);

	int32_t index = 0;
	while (!signal_caught && index < 50)
	{
		ml::TestRequest * r = new ml::TestRequest();
		r->setValue(index);
		r->setFunc(&logFunc);
		ML_Dispatcher.addRequest(r);
		index++;
	}

	std::this_thread::sleep_for(ml::Seconds(5));

	ML_Dispatcher.stop();

	return ml::Debug::pause(0);
}

#endif // !_TESTING_HPP_
