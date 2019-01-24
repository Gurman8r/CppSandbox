#ifndef _PARSER_H_
#define _PARSER_H_

#include <MemeScript/TokenList.h>
#include <MemeScript/AST_Block.h>
#include <MemeScript/AST_Stmt.h>
#include <MemeScript/AST_Expr.h>

namespace ml
{
	class ML_SCRIPT_API Parser final
		: public ITrackable
	{
	public:
		using ToksList = std::vector<TokenList>;

	public:
		Parser() {}
		~Parser() {}

		static ToksList	SplitStatements(const TokenList& toks);
		static bool		InfixToPostfix(const TokenList& ifx, TokenList& pfx, bool show);
		static bool		MakeOperator(const Token& lhs, const Token& rhs, Operator& op);

		// Upper Level
		AST_Block*	genAST(const TokenList& tokens) const;
		AST_Node*	genNode(AST_Node* root, const TokenList& toks) const;
		AST_Stmt*	genStatement(const TokenList& toks) const;
		AST_Expr*	genComplex(const TokenList& toks) const;
		AST_Expr*	genSimple(const Token& token) const;

		// Simple Expressions
		AST_Bool*	genBool(const Token& token) const;
		AST_Flt*	genFlt(const Token& token) const;
		AST_Int*	genInt(const Token& token) const;
		AST_Name*	genName(const Token& token) const;
		AST_String*	genStr(const Token& token) const;

		// Complex Expressions
		AST_Array*	genArray(const TokenList& toks) const;
		AST_Assign*	genAssign(const TokenList& toks) const;
		AST_Call*	genCall(const TokenList& toks) const;
		AST_Func*	genFunc(const TokenList& toks) const;
		AST_Input*	genInput(const TokenList& toks) const;
		AST_BinOp*	genBinOp(const TokenList& toks) const;
		AST_Subscr*	genSubscr(const TokenList& toks) const;

		// Expression Containers
		AST_Array::Values genArrayElements(const TokenList& toks) const;
		AST_Call::Params  genCallParams(const TokenList& toks) const;
		AST_Func::Params  genFuncParams(const TokenList& toks) const;

		// Settings
		inline Parser & showToks(bool value) { m_showToks = value; return (*this); }
		inline Parser & showTree(bool value) { m_showTree = value; return (*this); }
		inline Parser & showItoP(bool value) { m_showItoP = value; return (*this); }

	private:
		bool m_showToks;
		bool m_showTree;
		bool m_showItoP;
	};
}

#endif // !_PARSER_H_
