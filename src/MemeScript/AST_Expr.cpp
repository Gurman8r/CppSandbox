#include <MemeScript/AST_Expr.h>
#include <MemeScript/Interpreter.h>

#include <MemeCore/DebugUtility.h>
#include <iostream>

namespace ml
{
	AST_Expr::AST_Expr(AST_Expr::Type type)
		: AST_Stmt(AST_Stmt::Type::ST_Expr)
		, exprType(type)
	{
	}

	AST_Expr::~AST_Expr()
	{
	}

	std::ostream & AST_Expr::display(std::ostream & out) const
	{
		return AST_Stmt::display(out);
	}

	bool AST_Expr::run()
	{
		Var v = evaluate();
		if (v.isValid() && !v.isPointerType())
		{
			//out() << v << endln;
			return true;
		}
		return Debug::LogError("AST_Expr : Invalid Syntax: \'{0}\'", v.textValue());
	}


	//	Expressions
	/* * * * * * * * * * * * * * * * */

	// Array
	AST_Array::AST_Array(const Values & values)
		: AST_Expr(EX_Array)
		, values(values)
	{
		for (auto v : values)
			addChild(v);
	}

	AST_Array::~AST_Array()
	{
		values.clear();
	}

	std::ostream & AST_Array::display(std::ostream & out) const
	{
		out << evaluate();

		return out;
	}

	Var AST_Array::evaluate() const
	{
		TokenList items;

		for (auto it = values.begin(); it != values.end(); it++)
		{
			items.push_back((*it)->evaluate().tokensValue());
		}

		return Var().arrayValue(items);
	}


	// Assignment
	AST_Assign::AST_Assign(Operator op, AST_Name * name, AST_Expr * expr)
		: AST_Expr(AST_Expr::Type::EX_Assign)
		, op(op)
		, name(name)
		, expr(expr)
	{
		addChild(name);
		addChild(expr);
	}

	AST_Assign::~AST_Assign()
	{
	}

	std::ostream & AST_Assign::display(std::ostream & out) const
	{
		return out << (*name) << " " << op << " " << (*expr);
	}

	Var AST_Assign::evaluate() const
	{
		if (op == OperatorType::OP_SET)
		{
			if (Var * v = block()->setv(name->value, expr->evaluate()))
			{
				return (*v);
			}
		}
		else if (Var * v = block()->getv(name->value))
		{
			switch (op.type)
			{
			case OperatorType::OP_ADD:
				return v->Add(expr->evaluate());

			case OperatorType::OP_SUB:
				return v->Sub(expr->evaluate());

			case OperatorType::OP_MUL:
				return v->Mul(expr->evaluate());

			case OperatorType::OP_DIV:
				return v->Div(expr->evaluate());

			case OperatorType::OP_POW:
				return v->Pow(expr->evaluate());
			}
		}
		return Var().errorValue("AST_Assign : Set Value Failed");
	}

	bool AST_Assign::run()
	{
		if (evaluate().isValid())
		{
			return runNext();
		}
		return Debug::LogError("AST_Assign : Mod Value Failed");
	}


	// Bool
	AST_Bool::AST_Bool(bool value)
		: AST_Expr(AST_Expr::Type::EX_Bool)
		, value(value)
	{
	}

	AST_Bool::~AST_Bool()
	{
	}

	std::ostream & AST_Bool::display(std::ostream & out) const
	{
		out << Var().boolValue(value);
		return out;
	}

	Var AST_Bool::evaluate() const
	{
		return Var().boolValue(value);
	}


	// Call
	AST_Call::AST_Call(AST_Name * name, const Params& args)
		: AST_Expr(AST_Expr::Type::EX_Call)
		, name(name)
		, args(args)
	{
		addChild(name);
		for (auto p : args)
			addChild(p);
	}

	AST_Call::~AST_Call()
	{
		args.clear();
	}

	std::ostream & AST_Call::display(std::ostream & out) const
	{
		out << (FG::Green | BG::Black)
			<< name->value << "("
			<< FMT();
		Params::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << *(*it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << (FG::Green | BG::Black) << ")"
			<< FMT();
		return out;
	}

	AST_Func * AST_Call::getFunc() const
	{
		if (AST_Block* b = block())
		{
			if (Var * v = b->getv(name->value))
			{
				if (v->isFuncType())
				{
					return b->getFunc(v->textValue());
				}
			}
		}
		return NULL;
	}

	Var AST_Call::evaluate() const
	{
		if (AST_Func* f = getFunc())
		{
			if (AST_Block* b = f->nextAs<AST_Block>())
			{
				if (f->args.size() == args.size())
				{
					for (size_t i = 0; i < f->args.size(); i++)
					{
						if (!b->setv(f->args[i]->value, args[i]->evaluate()))
						{
							return Var().errorValue("AST_Call : {0} Set Arg Failed",
								f->args[i]->value);
						}
					}

					if (b->runFirst())
					{
						for (size_t i = 0; i < f->args.size(); i++)
						{
							if (!b->delv(f->args[i]->value))
							{
								return Var().errorValue("AST_Call : {0} Del Arg Failed",
									f->args[i]->value);
							}
						}

						return b->evaluate();
					}
					else
					{
						return Var().errorValue("AST_Call : {0} Failed Running Comparison Body",
							f->name);
					}
				}
				else
				{
					return Var().errorValue("AST_Call : Expected {0} args, recieved {1}",
						f->args.size(), args.size());
				}
			}
			else
			{
				return Var().errorValue("AST_Call : {0} Comparison Body Not Found", f->name);
			}
		}
		else
		{
			return Var().errorValue("AST_Call : {0} Comparison Not Found", name->value);
		}
	}

	bool AST_Call::run()
	{
		Var v;
		if ((v = evaluate()).isErrorType())
		{
			return Debug::LogError("AST_Call : Call Failed {0}", v);
		}
		return runNext();
	}


	// Float
	AST_Flt::AST_Flt(float value)
		: AST_Expr(AST_Expr::Type::EX_Flt)
		, value(value)
	{
	}

	AST_Flt::~AST_Flt()
	{
	}

	std::ostream & AST_Flt::display(std::ostream & out) const
	{
		out << Var().floatValue(value);
		return out;
	}

	Var AST_Flt::evaluate() const
	{
		return Var().floatValue(value);
	}


	// Function
	AST_Func::AST_Func(const std::string& name, const Params& args)
		: AST_Expr(AST_Expr::Type::EX_Func)
		, name(name)
		, args(args)
	{
		for (auto p : args)
			addChild(p);
	}

	AST_Func::~AST_Func()
	{
	}

	std::ostream & AST_Func::display(std::ostream & out) const
	{
		out << (FG::White | BG::DarkGray) << name << FMT() << " = [](";
		Params::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << *(*it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << ")";
		return out;
	}

	Var AST_Func::evaluate() const
	{
		return Var();
	}

	bool AST_Func::run()
	{
		if (block()->addFunc(name, this))
		{
			return runNext();
		}
		return Debug::LogError("AST_Func : Run Failed");
	}


	// Input
	AST_Input::AST_Input()
		: AST_Expr(AST_Expr::Type::EX_Input)
	{
	}

	AST_Input::~AST_Input()
	{
	}

	std::ostream & AST_Input::display(std::ostream & out) const
	{
		out << "input()";
		return out;
	}

	Var AST_Input::evaluate() const
	{
#ifdef ML_DEBUG

		Debug::out() << "$: ";
		std::string line;
		std::getline(Debug::in(), line);
		return Var().stringValue(line);
#else
		return Var();
#endif
	}


	// Int
	AST_Int::AST_Int(int32_t value)
		: AST_Expr(AST_Expr::Type::EX_Int)
		, value(value)
	{
	}

	AST_Int::~AST_Int()
	{
	}

	std::ostream & AST_Int::display(std::ostream & out) const
	{
		out << Var().intValue(value);
		return out;
	}

	Var AST_Int::evaluate() const
	{
		return Var().intValue(value);
	}


	// Name
	AST_Name::AST_Name(const std::string & value)
		: AST_Expr(AST_Expr::Type::EX_Name)
		, value(value)
	{
	}

	AST_Name::~AST_Name()
	{
	}

	std::ostream & AST_Name::display(std::ostream & out) const
	{
		if (AST_Block* b = block())
		{
			if (Var * v = block()->getv(value))
			{
				out << (*v);
			}
			else
			{
				out << Var().pointerValue({ block()->getID(), value });
			}
		}
		else
		{
			out << Var().errorValue("AST_Name : Null Ref: {0}", value);
		}
		return out;
	}

	Var AST_Name::evaluate() const
	{
		if (AST_Block* b = block())
		{
			if (Var * v = block()->getv(value))
			{
				return (*v);
			}
			else
			{
				return *block()->setv(value, Var().pointerValue({ block()->getID(), value }));
			}
		}

		return Var().errorValue("AST_Name : Null reference");
	}


	// BinOp
	AST_BinOp::AST_BinOp(const Operator & op, AST_Expr * lhs, AST_Expr * rhs)
		: AST_Expr(AST_Expr::Type::EX_Oper)
		, op(op)
		, lhs(lhs)
		, rhs(rhs)
	{
		addChild(lhs);
		addChild(rhs);
	}

	AST_BinOp::~AST_BinOp()
	{
	}

	std::ostream & AST_BinOp::display(std::ostream & out) const
	{
		out << "(" << *lhs << " " << op << " " << *rhs << ")";
		return out;
	}

	Var AST_BinOp::evaluate() const
	{
		if (lhs && rhs)
		{
			switch (op.type)
			{
			case OperatorType::OP_ADD:
				return lhs->evaluate() + rhs->evaluate();
			case OperatorType::OP_SUB:
				return lhs->evaluate() - rhs->evaluate();
			case OperatorType::OP_MUL:
				return lhs->evaluate() * rhs->evaluate();
			case OperatorType::OP_DIV:
				return lhs->evaluate() / rhs->evaluate();
			case OperatorType::OP_POW:
				return lhs->evaluate() ^ rhs->evaluate();

			case OperatorType::OP_EQU:
				return Var().boolValue(lhs->evaluate() == rhs->evaluate());
			case OperatorType::OP_NEQ:
				return Var().boolValue(lhs->evaluate() != rhs->evaluate());
			case OperatorType::OP_LT:
				return Var().boolValue(lhs->evaluate() < rhs->evaluate());
			case OperatorType::OP_GT:
				return Var().boolValue(lhs->evaluate() > rhs->evaluate());
			case OperatorType::OP_LTE:
				return Var().boolValue(lhs->evaluate() <= rhs->evaluate());
			case OperatorType::OP_GTE:
				return Var().boolValue(lhs->evaluate() >= rhs->evaluate());

			case OperatorType::OP_AND:
				return Var().boolValue(lhs->evaluate() && rhs->evaluate());
			case OperatorType::OP_OR:
				return Var().boolValue(lhs->evaluate() || rhs->evaluate());
			}
		}
		return Var().errorValue("AST_Oper : Invalid Operation {0} {1} {2}", *lhs, op, *rhs);
	}


	// String
	AST_String::AST_String(const std::string & value)
		: AST_Expr(AST_Expr::Type::EX_Str)
		, value(value)
	{
	}

	AST_String::~AST_String()
	{
	}

	std::ostream & AST_String::display(std::ostream & out) const
	{
		return out << Var().stringValue(value);
	}

	Var AST_String::evaluate() const
	{
		return Var().stringValue(value);
	}


	// Struct
	AST_Struct::AST_Struct()
		: AST_Expr(AST_Expr::Type::EX_Struct)
	{
	}

	AST_Struct::~AST_Struct()
	{
	}

	std::ostream & AST_Struct::display(std::ostream & out) const
	{
		return out;
	}

	Var AST_Struct::evaluate() const
	{
		return Var();
	}


	// Subscript
	AST_Subscr::AST_Subscr(AST_Name* name, AST_Expr * index)
		: AST_Expr(AST_Expr::Type::EX_Subscr)
		, name(name)
		, index(index)
	{
		addChild(name);
		addChild(index);
	}

	AST_Subscr::~AST_Subscr()
	{
	}

	std::ostream & AST_Subscr::display(std::ostream & out) const
	{
		out << "" << name->value << "[" << *index << "]";
		return out;
	}

	Var AST_Subscr::evaluate() const
	{
		if (AST_Block* b = block())
		{
			if (Var * a = b->getv(name->value))
			{
				Var v = index->evaluate();
				if (v.isIntType())
				{
					return a->elemValue(v.intValue());
				}
				else
				{
					return Var().errorValue("AST_Subscr : Could not evaluate expression to index: \'{0}\'", v.textValue());
				}
			}
			return Var().errorValue("AST_Subscr : Null reference {0}", name->value);
		}
		return Var().errorValue("AST_Subscr : Null Block {0}", name->value);
	}


	// System
	AST_Command::AST_Command(AST_Expr* expr)
		: AST_Expr(AST_Expr::Type::EX_Sys)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_Command::~AST_Command()
	{
	}

	std::ostream & AST_Command::display(std::ostream & out) const
	{
		return out << "command(" << (*expr) << ")";
	}

	Var AST_Command::evaluate() const
	{
		return ML_Interpreter.execCommand(expr->evaluate().textValue());
	}

	bool AST_Command::run()
	{
		if (evaluate().isValid())
		{
			return runNext();
		}
		return Debug::LogError("AST_Sys : Call Failed");
	}
}