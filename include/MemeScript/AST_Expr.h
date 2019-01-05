#ifndef _AST_EXPR_H_
#define _AST_EXPR_H_

#include <MemeScript/AST_Stmt.h>
#include <MemeScript/Operator.h>

namespace ml
{
	class ML_SCRIPT_API AST_Expr : public AST_Stmt
	{
	public:
		enum Type : int
		{
			EX_INVALID = -1,
			EX_Array,
			EX_Assign,
			EX_Bool = AST_Stmt::Type::MAX_STMT_TYPE + 1,
			EX_Call,
			EX_Flt,
			EX_Func,
			EX_Input,
			EX_Int,
			EX_Name,
			EX_Oper,
			EX_Str,
			EX_Struct,
			EX_Subscr,
			EX_Sys,
			MAX_EXPR_TYPE
		};

		AST_Expr::Type exprType;

		AST_Expr(AST_Expr::Type type);
		virtual ~AST_Expr();

		virtual std::ostream& display(std::ostream& out) const;

		virtual Var evaluate() const = 0;

		virtual bool run();
	};

	//	Expressions
	/* * * * * * * * * * * * * * * * */

	struct ML_SCRIPT_API AST_Array : public AST_Expr
	{
		using Values = std::vector<AST_Expr*>;

		Values values;

		AST_Array(const Values& values);
		~AST_Array();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Bool
	struct ML_SCRIPT_API AST_Bool : public AST_Expr
	{
		bool value;

		AST_Bool(bool value);
		~AST_Bool();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Float
	struct ML_SCRIPT_API AST_Flt : public AST_Expr
	{
		float value;

		AST_Flt(float value);
		~AST_Flt();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Function
	struct ML_SCRIPT_API AST_Func : public AST_Expr
	{
		using Params = std::vector<AST_Name*>;

		std::string name;
		Params	args;

		AST_Func(const std::string& name, const Params& args);
		~AST_Func();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
		bool run() override;
	};

	// Input
	struct ML_SCRIPT_API AST_Input : public AST_Expr
	{
		AST_Input();
		~AST_Input();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Int
	struct ML_SCRIPT_API AST_Int : public AST_Expr
	{
		int value;

		AST_Int(int value);
		~AST_Int();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};


	// Name
	struct ML_SCRIPT_API AST_Name : public AST_Expr
	{
		std::string value;

		AST_Name(const std::string& value);
		~AST_Name();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Assignment
	struct ML_SCRIPT_API AST_Assign : public AST_Expr
	{
		Operator	op;
		AST_Name*	name;
		AST_Expr*	expr;

		AST_Assign(Operator op, AST_Name* name, AST_Expr* expr);
		~AST_Assign();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
		bool run() override;
	};

	// Call
	struct ML_SCRIPT_API AST_Call : public AST_Expr
	{
		using Params = std::vector<AST_Expr*>;

		AST_Name*	name;
		Params		args;

		AST_Call(AST_Name* name, const Params& args);
		~AST_Call();

		std::ostream& display(std::ostream& out) const override;
		Var			evaluate() const override;
		AST_Func*	getFunc() const;
		bool		run() override;
	};


	// Operation
	struct ML_SCRIPT_API AST_Oper : public AST_Expr
	{
		Operator op;
		AST_Expr* lhs;
		AST_Expr* rhs;

		AST_Oper(const Operator& op, AST_Expr* lhs, AST_Expr* rhs);
		~AST_Oper();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// String
	struct ML_SCRIPT_API AST_String : public AST_Expr
	{
		std::string value;

		AST_String(const std::string& value);
		~AST_String();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Struct
	struct ML_SCRIPT_API AST_Struct : public AST_Expr
	{
		AST_Struct();
		~AST_Struct();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Subscript
	struct ML_SCRIPT_API AST_Subscr : public AST_Expr
	{
		AST_Name* name;
		AST_Expr* index;

		AST_Subscr(AST_Name* name, AST_Expr* index);
		~AST_Subscr();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// System
	struct ML_SCRIPT_API AST_Sys : public AST_Expr
	{
		AST_String* str;

		AST_Sys(AST_String* str);
		~AST_Sys();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
		bool run() override;
	};
}

#endif // !_AST_EXPR_H_
