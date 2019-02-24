#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <MemeScript/Rule.hpp>

namespace ml
{
	class ML_SCRIPT_API Parser final
		: public ITrackable
	{
		friend class Interpreter;

	public:
		using SyntaxTree = List<TokenList>;
		using RuleMap	 = HashMap<const std::type_info *, Rule *>;

	private:
		Parser();
		~Parser();

		void install_expressions();
		void install_statements();

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		static SyntaxTree SplitStatements(const TokenList & toks);
		static bool	InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show);

		/* * * * * * * * * * * * * * * * * * * * */

		AST_Block *	genAST(const TokenList & tokens) const;
		AST_Node *	genNode(AST_Node* root, const TokenList & toks) const;
		AST_Stmt *	genStatement(const TokenList & toks) const;
		AST_Expr *	genExpression(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

		AST_BinOp *	postfixToBinOp(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

		AST_Array::Values genArrayElements(const TokenList & toks) const;
		AST_Call::Params  genCallParams(const TokenList & toks) const;
		AST_Func::Params  genFuncParams(const TokenList & toks) const;

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		inline Parser & showToks(bool value) { m_showToks = value; return (*this); }
		inline Parser & showTree(bool value) { m_showTree = value; return (*this); }
		inline Parser & showItoP(bool value) { m_showItoP = value; return (*this); }

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
