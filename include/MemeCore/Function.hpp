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

	template <typename Fun>
	class VoidFun final : public Function
	{
		Fun m_fun;

	public:
		VoidFun(Fun fun)
			: m_fun(fun)
		{
		}

		inline void run() override { return m_fun(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename Fun, typename Arg>
	class ArgFun final : public Function
	{
		Fun	m_fun;
		Arg	m_arg;

	public:
		ArgFun(Fun fun, Arg arg)
			: m_fun(fun)
			, m_arg(arg)
		{
		}

		inline void run() override { return m_fun(m_arg); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename Fun = void(T::*)()>
	class MemberFun final : public Function
	{
		Fun	m_fun;
		T *	m_obj;

	public:
		MemberFun(Fun fun, T * obj)
			: m_fun(fun)
			, m_obj(obj)
		{
		}

		inline void run() override { return (m_obj->*m_fun)(); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <typename T, typename Arg, typename Fun = void(T::*)()>
	class MemberArgFun final : public Function
	{
		Fun	m_fun;
		T *	m_obj;
		Arg	m_arg;

	public:
		MemberArgFun(Fun fun, T * obj, Arg arg)
			: m_fun(fun)
			, m_obj(obj)
			, m_arg(arg)
		{
		}

		inline void run() override { return (m_obj->*m_fun)(m_arg); }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_FUNCTION_HPP_
