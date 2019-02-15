#ifndef _FUNCTION_HPP_
#define _FUNCTION_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Function
		: public ITrackable
	{
	public:
		virtual ~Function() {}
		
		virtual void run() = 0;
		
		inline void operator()() { return run(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename F>
	class VoidFun final : public Function
	{
	public:
		VoidFun(F fun)
			: m_fun(fun)
		{
		}

		inline void run() override { return m_fun(); }

	private:
		F m_fun;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename A>
	class ArgFun final : public Function
	{
	public:
		using F = void(*)(A);

	public:
		ArgFun(F fun, A arg)
			: m_fun(fun)
			, m_arg(arg)
		{
		}

		void run() override { return m_fun(m_arg); }

	private:
		F m_fun;
		A m_arg;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T>
	class MemberFun final : public Function
	{
	public:
		using F = void(T::*)();

	public:
		MemberFun(F fun, T * obj)
			: m_fun(fun)
			, m_obj(obj)
		{
		}

		void run() override { return (m_obj->*m_fun)(); }

	private:
		F	m_fun;
		T *	m_obj;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename A>
	class MemberArgFun final : public Function
	{
	public:
		using F = void(T::*)(A *);

	public:
		MemberArgFun(F fun, T * obj, A * arg)
			: m_fun(fun)
			, m_obj(obj)
			, m_arg(arg)
		{
		}

		void run() override { return (m_obj->*m_fun)(m_arg); }

	private:
		F	m_fun;
		T *	m_obj;
		A * m_arg;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_FUNCTION_HPP_
