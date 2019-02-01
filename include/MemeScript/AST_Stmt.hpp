#ifndef _AST_STMT_HPP_
#define _AST_STMT_HPP_

#include <MemeScript/AST_Block.hpp>

namespace ml
{
	class  AST_Expr;
	struct AST_Assign;
	struct AST_Bool;
	struct AST_Call;
	struct AST_Flt;
	struct AST_Func;
	struct AST_Int;
	struct AST_Name;
	struct AST_Call;
	struct AST_BinOp;
	struct AST_String;

	class ML_SCRIPT_API AST_Stmt : public AST_Node
	{
	public:
		enum Type
		{
			ST_INVALID = -1,
			ST_Elif,
			ST_Else,
			ST_Expr,
			ST_For,
			ST_Delete,
			ST_If,
			ST_Include,
			ST_Print,
			ST_Return,
			ST_While,
			MAX_STMT_TYPE
		};

		AST_Stmt::Type stmtType;

	public:
		AST_Stmt(AST_Stmt::Type stmt);
		virtual ~AST_Stmt();

		virtual std::ostream& display(std::ostream& out) const;

		AST_Block*	block() const;
		virtual bool run() = 0;
	};


	//	Statements
	/* * * * * * * * * * * * * * * * */

	// Elif
	struct ML_SCRIPT_API AST_Elif : public AST_Stmt
	{
		AST_Expr* expr;

		AST_Elif(AST_Expr* expr);
		~AST_Elif();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// Else
	struct ML_SCRIPT_API AST_Else : public AST_Stmt
	{
		AST_Else();
		~AST_Else();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// For
	struct ML_SCRIPT_API AST_For : public AST_Stmt
	{
		AST_Assign* assn;
		AST_Expr* expr;
		AST_Stmt* stmt;

		AST_For(AST_Assign* assign, AST_Expr* expr, AST_Stmt* stmt);
		~AST_For();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// Free
	struct ML_SCRIPT_API AST_Delete : public AST_Stmt
	{
		AST_Name* name;

		AST_Delete(AST_Name* name);
		~AST_Delete();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// If
	struct ML_SCRIPT_API AST_If : public AST_Stmt
	{
		AST_Expr* expr;

		AST_If(AST_Expr* expr);
		~AST_If();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// Include
	struct ML_SCRIPT_API AST_Include : public AST_Stmt
	{
		AST_String* str;
		
		AST_Include(AST_String* str);
		~AST_Include();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// Print
	struct ML_SCRIPT_API AST_Print : public AST_Stmt
	{
		AST_Expr* expr;
		bool endl;

		AST_Print(AST_Expr* expr);
		AST_Print(AST_Expr* expr, bool endl);
		~AST_Print();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// Return
	struct ML_SCRIPT_API AST_Return : public AST_Stmt
	{
		AST_Expr* expr;

		AST_Return(AST_Expr* expr);
		~AST_Return();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};

	// While
	struct ML_SCRIPT_API AST_While : public AST_Stmt
	{
		AST_Expr* expr;

		AST_While(AST_Expr* expr);
		~AST_While();

		std::ostream& display(std::ostream& out) const override;
		bool run() override;
	};
}

#endif // !_AST_STMT_HPP_