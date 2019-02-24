#ifndef _RULE_HPP_
#define _RULE_HPP_

#include <MemeScript/TokenList.hpp>
#include <MemeScript/AST_Block.hpp>
#include <MemeScript/AST_Stmt.hpp>
#include <MemeScript/AST_Expr.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Maker
		: public ITrackable
	{
	public:
		virtual ~Maker() {}

		virtual AST_Node * run(const TokenList & toks) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class _Node,
		class _Toks = const TokenList &,
		typename _Fun = _Node * (*)(_Toks)
	>
	class NodeMaker : public Maker
	{
		_Fun m_fun;

	public:
		NodeMaker(_Fun fun)
			: m_fun(fun)
		{
		}
		~NodeMaker() {}

		inline _Node * run(_Toks toks) const override { return m_fun(toks); }
	};

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API Rule
		: public ITrackable
	{
	public:
		Rule() 
			: m_maker(NULL)
		{
		}
		Rule(Maker * maker)
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
		Maker * m_maker;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_RULE_HPP_
