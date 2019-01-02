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

		enum Flags : uint16_t
		{
			None	 = (0 << 0),
			ShowToks = (1 << 0),
			ShowTree = (1 << 1),
		};
		inline friend Flags operator|(const Parser::Flags& lhs, const Parser::Flags& rhs)
		{
			return static_cast<Parser::Flags>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
		}

	public:
		Parser();
		~Parser();

		static ToksList	SplitStatements(const TokenList& toks);
		static bool		InfixToPostfix(const TokenList& ifx, TokenList& pfx);
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
		AST_Str*	genStr(const Token& token) const;

		// Complex Expressions
		AST_Array*	genArray(const TokenList& toks) const;
		AST_Assign*	genAssign(const TokenList& toks) const;
		AST_Call*	genCall(const TokenList& toks) const;
		AST_Func*	genFunc(const TokenList& toks) const;
		AST_Input*	genInput(const TokenList& toks) const;
		AST_Oper*	genOper(const TokenList& toks) const;
		AST_Subscr*	genSubscr(const TokenList& toks) const;

		// Expression Containers
		AST_Array::Values genArrayElements(const TokenList& toks) const;
		AST_Call::Params  genCallParams(const TokenList& toks) const;
		AST_Func::Params  genFuncParams(const TokenList& toks) const;

		inline Flags getFlags() const { return m_flags; }
		inline void	 setFlags(Flags value) { m_flags = value; }

	private:
		Flags m_flags;
	};
}

#endif // !_PARSER_H_
