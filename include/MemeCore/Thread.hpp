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
			: m_fun(new VoidFun<F>(fun))
			, m_thr(NULL)
		{
		}

		template <typename F, typename A>
		Thread(F fun, A arg)
			: m_fun(new ArgFun<F, A>(fun, arg))
			, m_thr(NULL)
		{
		}

		template <typename C, typename F = void(C::*)()>
		Thread(F fun, C * obj)
			: m_fun(new MemberFun<C, F>(fun, obj))
			, m_thr(NULL)
		{
		}

		template <typename C, typename A, typename F = void(C::*)()>
		Thread(F fun, C * obj, A arg)
			: m_fun(new MemberArgFun<C, A, F>(fun, obj, arg))
			, m_thr(NULL)
		{
		}

	public:
		Thread & cleanup();
		Thread & detatch();
		Thread & launch();
		Thread & swap(Thread & other);
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
