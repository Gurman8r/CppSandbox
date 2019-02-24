#ifndef _RULE_HPP_
#define _RULE_HPP_

#include <MemeScript/TokenList.hpp>
#include <MemeScript/AST_Block.hpp>
#include <MemeScript/AST_Stmt.hpp>
#include <MemeScript/AST_Expr.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API INodeMaker : public ITrackable
	{
	public:
		virtual ~INodeMaker() {}

		virtual AST_Node * run(const TokenList & toks) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		typename _Ret,
		typename _Arg  = const TokenList &,
		typename _Fun = _Ret * (*)(_Arg)
	>
	class NodeMaker : public INodeMaker
	{
		_Fun m_fun;

	public:
		NodeMaker()
			: m_fun(NULL)
		{
		}
		NodeMaker(_Fun fun)
			: m_fun(fun)
		{
		}
		~NodeMaker() {}

		inline _Ret * run(_Arg toks) const override 
		{ 
			return m_fun(toks); 
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Rule final : public ITrackable
	{
	public:
		Rule() 
			: m_maker(NULL)
		{
		}
		Rule(INodeMaker * maker)
			: m_maker(maker)
		{
		}
		~Rule() 
		{
			if (m_maker)
			{
				delete m_maker;
				m_maker = NULL;
			}
		}

		inline AST_Node * run(const TokenList & toks) const
		{
			if (m_maker)
			{
				return m_maker->run(toks);
			}
			return NULL;
		}

		inline AST_Node * operator()(const TokenList & toks) const
		{
			return run(toks);
		}

	private:
		INodeMaker * m_maker;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_RULE_HPP_
