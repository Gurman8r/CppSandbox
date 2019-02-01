#ifndef _AST_EXPR_HPP_
#define _AST_EXPR_HPP_

#include <MemeScript/AST_Stmt.hpp>
#include <MemeScript/Operator.hpp>

namespace ml
{
	class ML_SCRIPT_API AST_Expr : public AST_Stmt
	{
	public:
		enum Type : int32_t
		{
			EX_INVALID = -1,
			EX_Array,
			EX_Assign,
			EX_Bool = AST_Stmt::Type::MAX_STMT_TYPE + 1,
			EX_Call,
			EX_Float,
			EX_Func,
			EX_Input,
			EX_Int,
			EX_Name,
			EX_BinOp,
			EX_String,
			EX_Struct,
			EX_Subscr,
			EX_Command,
			EX_SizeOf,
			EX_TypeID,
			EX_TypeName,
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

		String name;
		Params	args;

		AST_Func(const String& name, const Params& args);
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
		size_t value;

		AST_Int(size_t value);
		~AST_Int();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};


	// Name
	struct ML_SCRIPT_API AST_Name : public AST_Expr
	{
		String value;

		AST_Name(const String& value);
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
	struct ML_SCRIPT_API AST_BinOp : public AST_Expr
	{
		Operator op;
		AST_Expr* lhs;
		AST_Expr* rhs;

		AST_BinOp(const Operator& op, AST_Expr* lhs, AST_Expr* rhs);
		~AST_BinOp();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// String
	struct ML_SCRIPT_API AST_String : public AST_Expr
	{
		String value;

		AST_String(const String& value);
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

	// Command
	struct ML_SCRIPT_API AST_Command : public AST_Expr
	{
		AST_Expr* expr;

		AST_Command(AST_Expr* expr);
		~AST_Command();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
		bool run() override;
	};

	// Size Of
	struct ML_SCRIPT_API AST_SizeOf : public AST_Expr
	{
		AST_Expr * expr;

		AST_SizeOf(AST_Expr * expr);
		~AST_SizeOf();

		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Type ID
	struct ML_SCRIPT_API AST_TypeID : public AST_Expr
	{
		AST_Expr * expr;
		
		AST_TypeID(AST_Expr * expr);
		~AST_TypeID();
		
		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};

	// Type Name
	struct ML_SCRIPT_API AST_TypeName : public AST_Expr
	{
		AST_Expr * expr;
		
		AST_TypeName(AST_Expr * expr);
		~AST_TypeName();
		
		std::ostream& display(std::ostream& out) const override;
		Var evaluate() const override;
	};
}

#endif // !_AST_EXPR_HPP_
