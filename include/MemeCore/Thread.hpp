#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#include <MemeCore/Function.hpp>
#include <MemeCore/INonCopyable.hpp>

namespace ml
{
	class ML_CORE_API Thread 
		: public ITrackable
		, public INonCopyable
	{
	public:
		Thread();
		~Thread();

		template <typename T>
		Thread(T fun)
			: m_fun(new VoidFun<T>(fun))
		{
		}

		template <typename T, typename A>
		Thread(T fun, A arg)
			: m_fun(new ArgFun<T, A>(fun, arg))
		{
		}

		template <typename T>
		Thread(void(T::*fun)(), T * obj)
			: m_fun(new MemberFun<T>(fun, obj))
		{
		}

		template <typename T, typename A>
		Thread(void(T::*fun)(), T * obj, A * arg)
			: m_fun(new MemberArgFun<T, A>(fun, obj, arg))
		{
		}

	public:
		Thread & cleanup();
		Thread & detatch();
		Thread & launch();
		Thread & join();
		Thread & swap(Thread & other);
		Thread & terminate();

		inline void operator()() { launch(); }

	private:
		void run();

		std::thread * m_thread;
		Function *	  m_fun;
	};
}

#endif // !_THREAD_HPP_
