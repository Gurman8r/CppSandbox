#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <MemeScript/Rule.hpp>

namespace ml
{
	class ML_SCRIPT_API Parser final
		: public ITrackable
	{
	public:
		using RuleMap = HashMap<const std::type_info *, Rule *>;

	public:
		Parser();
		~Parser();

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static bool	InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show);

		TokenList infixToPostfix(const TokenList & ifx, bool show) const;

		/* * * * * * * * * * * * * * * * * * * * */

		AST_Block *	genFromList(const TokenList & value) const;
		AST_Block *	genFromTree(const TokenTree & value) const;
		AST_Node *	genNode(AST_Node * root, const TokenList & toks) const;
		AST_Stmt *	genStatement(const TokenList & toks) const;
		AST_Expr *	genExpression(const TokenList & toks) const;
		AST_BinOp *	genNestedBinOp(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

		void install_statements();
		void install_expressions();

		/* * * * * * * * * * * * * * * * * * * * */

		AST_Array::Elems genArrayElems(const TokenList & toks) const;
		AST_Call::Params genCallParams(const TokenList & toks) const;
		AST_Func::Params genFuncParams(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		inline Parser & showToks(bool value) { m_showToks = value; return (*this); }
		inline Parser & showTree(bool value) { m_showTree = value; return (*this); }
		inline Parser & showItoP(bool value) { m_showItoP = value; return (*this); }

		inline bool showToks() const { return m_showToks; }
		inline bool showTree() const { return m_showTree; }
		inline bool showItoP() const { return m_showItoP; }

	public:
		template <class T>
		inline Rule * install(INodeMaker * maker)
		{
			RuleMap::iterator it;
			if ((it = m_rules.find(&typeid(T))) == m_rules.end())
			{
				return (m_rules[&typeid(T)] = new Rule(maker));
			}
			return NULL;
		}

		template <class T>
		inline T * generate(const TokenList & toks) const
		{
			RuleMap::const_iterator it;
			if ((it = m_rules.find(&typeid(T))) != m_rules.end())
			{
				return (T *)it->second->run(toks);
			}
			return NULL;
		}

	private:
		bool m_showToks;
		bool m_showTree;
		bool m_showItoP;

		RuleMap m_rules;
	};
}

#endif // !_PARSER_HPP_
