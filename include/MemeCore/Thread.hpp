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
		using ID = typename std::thread::id;

	public:
		Thread();
		~Thread();

		template <typename F>
		Thread(F fun)
			: Thread()
		{
			update(new VoidFun<F>(fun));
		}

		template <typename F, typename A>
		Thread(F fun, A arg)
			: Thread()
		{
			update(new ArgFun<F, A>(fun, arg));
		}

		template <typename T, typename F = void(T::*)()>
		Thread(F fun, T * obj)
			: Thread()
		{
			update(new MemberFun<T, F>(fun, obj));
		}

		template <typename T, typename A, typename F = void(T::*)(A)>
		Thread(F fun, T * obj, A arg)
			: Thread()
		{
			update(new MemberArgFun<T, A, F>(fun, obj, arg));
		}

	public:
		Thread & cleanup();
		Thread & detatch();
		Thread & launch();
		Thread & swap(Thread & other);
		Thread & update(Function * fun);
		Thread & wait();

	public:
		bool	alive() const;
		void *	handle();
		ID		id() const;
		bool	joinable() const;

	private:
		void run();

		std::thread * m_thr;
		Function *	  m_fun;
	};
}

#endif // !_THREAD_HPP_
