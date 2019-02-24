#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <MemeScript/Rule.hpp>

namespace ml
{
	class ML_SCRIPT_API Parser final
		: public ITrackable
	{
	public:
		using ToksTree = std::vector<TokenList>;
		using RuleMap = HashMap<const std::type_info *, Rule *>;

	public:
		Parser();
		~Parser();

		// Static
		static ToksTree	SplitStatements(const TokenList & toks);
		static bool		InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show);
		static bool		MakeOperator(const Token& lhs, const Token& rhs, Operator& op);

		// Upper Level
		AST_Block *		genAST(const TokenList & tokens) const;
		AST_Node *		genNode(AST_Node* root, const TokenList & toks) const;
		AST_Stmt *		genStatement(const TokenList & toks) const;
		AST_Expr *		genExpression(const TokenList & toks) const;

		// Expressions
		AST_Array *		genArray(const TokenList & toks) const;
		AST_Assign *	genAssign(const TokenList & toks) const;
		AST_BinOp *		genBinOp1(const TokenList & toks) const;
		AST_BinOp *		genBinOp2(const TokenList & toks) const;
		AST_Call *		genCall(const TokenList & toks) const;
		AST_Func *		genFunc(const TokenList & toks) const;
		AST_Input *		genInput(const TokenList & toks) const;
		AST_Member *	genMember(const TokenList & toks) const;
		AST_Struct *	genStruct(const TokenList & toks) const;
		AST_Subscr *	genSubscr(const TokenList & toks) const;

		// Builtin Calls
		AST_Command *	genCommand(const TokenList & toks) const;
		AST_New *		genNew(const TokenList & toks) const;
		AST_NodeID *	genNodeID(const TokenList & toks) const;
		AST_SizeOf *	genSizeof(const TokenList & toks) const;
		AST_TypeID *	genTypeID(const TokenList & toks) const;
		AST_TypeName *	genTypeName(const TokenList & toks) const;

		// Expression Containers
		AST_Array::Values genArrayElements(const TokenList & toks) const;
		AST_Call::Params  genCallParams(const TokenList & toks) const;
		AST_Func::Params  genFuncParams(const TokenList & toks) const;

		// Settings
		inline Parser & showToks(bool value) { m_showToks = value; return (*this); }
		inline Parser & showTree(bool value) { m_showTree = value; return (*this); }
		inline Parser & showItoP(bool value) { m_showItoP = value; return (*this); }

	public:
		template <class T>
		inline Rule * addRule(Maker * maker)
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
