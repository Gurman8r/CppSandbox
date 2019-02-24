#include <MemeScript/AST_Expr.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	// Expression
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Expr::AST_Expr(int32_t type)
		: AST_Stmt(ST_Expr)
		, exprType(type)
	{
	}

	std::ostream & AST_Expr::display(std::ostream & out) const
	{
		return AST_Stmt::display(out);
	}

	Var AST_Expr::evaluate() const
	{
		return Var().voidValue();
	}

	bool AST_Expr::run()
	{
		const Var v = evaluate();
		if (v.isErrorType())
		{
			return Debug::logError("AST_Expr : Invalid Syntax: \'{0}\'", v.textValue());
		}
		return true;
	}

	
	// Array
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Array::AST_Array(const Values & values)
		: AST_Expr(EX_Array)
		, values(values)
	{
		for (auto v : values)
			addChild(v);
	}

	std::ostream & AST_Array::display(std::ostream & out) const
	{
		return out << evaluate();
	}

	Var AST_Array::evaluate() const
	{
		TokenList items;

		for (auto it = values.begin(); it != values.end(); it++)
		{
			items.push_back(( * it)->evaluate().tokensValue());
		}

		return Var().arrayValue(items);
	}


	// Assign
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Assign::AST_Assign(Operator op, AST_Expr * name, AST_Expr * expr)
		: AST_Expr(EX_Assign)
		, op(op)
		, name(name)
		, expr(expr)
	{
		addChild(name);
		addChild(expr);
	}

	std::ostream & AST_Assign::display(std::ostream & out) const
	{
		return out << ( * name) << " " << op << " " << ( * expr);
	}

	Var AST_Assign::evaluate() const
	{
		if (AST_Name * n = name->as<AST_Name>())
		{
			if (op == OperatorType::OP_SET)
			{
				if (Var * v = block()->setVar(n->value, expr->evaluate()))
				{
					return (*v);
				}
			}
			else if (Var * v = block()->getVar(n->value))
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

				case OperatorType::OP_MOD:
					return v->Mod(expr->evaluate());
				}
			}
		}
		return Var().errorValue("AST_Assign : Set value_type Failed");
	}

	bool AST_Assign::run()
	{
		if (evaluate().isValid())
		{
			return runNext();
		}
		return Debug::logError("AST_Assign : Mod value_type Failed");
	}


	// Bool
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Bool::AST_Bool(bool value)
		: AST_Expr(EX_Bool)
		, value(value)
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
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Call::AST_Call(AST_Name * name, const Params& args)
		: AST_Expr(EX_Call)
		, name(name)
		, args(args)
	{
		addChild(name);
		for (auto p : args)
			addChild(p);
	}

	std::ostream & AST_Call::display(std::ostream & out) const
	{
		out << (FG::Green | BG::Black)
			<< name->value << "("
			<< FMT();
		Params::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << * ( * it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << (FG::Green | BG::Black) << ")"
			<< FMT();
		return out;
	}

	AST_Func * AST_Call::getFunc() const
	{
		if (AST_Block * b = block())
		{
			if (Var * v = b->getVar(name->value))
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
		if (AST_Func * func = getFunc())
		{
			if (AST_Block * body = func->nextAs<AST_Block>())
			{
				if (func->args.size() == args.size())
				{
					for (size_t i = 0; i < func->args.size(); i++)
					{
						if (!body->setVar(
							func->args[i]->value,
							args[i]->evaluate()))
						{
							return Var().errorValue(
								"AST_Call : {0} Set Arg Failed",
								func->args[i]->value);
						}
					}

					if (body->runFirst())
					{
						for (size_t i = 0; i < func->args.size(); i++)
						{
							if (!body->delVar(func->args[i]->value))
							{
								return Var().errorValue(
									"AST_Call : {0} Del Arg Failed",
									func->args[i]->value);
							}
						}

						return body->evaluate();
					}
					else
					{
						return Var().errorValue(
							"AST_Call : {0} Failed Running Function Body",
							func->name);
					}
				}
				else
				{
					return Var().errorValue(
						"AST_Call : Expected {0} args, recieved {1}",
						func->args.size(), 
						args.size());
				}
			}
			else
			{
				return Var().errorValue(
					"AST_Call : {0} Function Body Not Found", 
					func->name);
			}
		}
		else
		{
			return Var().errorValue(
				"AST_Call : {0} Function Name Not Found",
				name->value);
		}
	}

	bool AST_Call::run()
	{
		Var v;
		if ((v = evaluate()).isErrorType())
		{
			return Debug::logError("AST_Call : Call Failed {0}", v);
		}
		return runNext();
	}


	// Float
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Float::AST_Float(float value)
		: AST_Expr(EX_Float)
		, value(value)
	{
	}

	std::ostream & AST_Float::display(std::ostream & out) const
	{
		out << Var().floatValue(value);
		return out;
	}

	Var AST_Float::evaluate() const
	{
		return Var().floatValue(value);
	}


	// Function
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Func::AST_Func(const String & name, const Params& args)
		: AST_Expr(EX_Func)
		, name(name)
		, args(args)
	{
		for (auto p : args)
			addChild(p);
	}

	std::ostream & AST_Func::display(std::ostream & out) const
	{
		out << (FG::White | BG::DarkGray) << name << FMT() << " = [](";
		Params::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << * ( * it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << ")";
		return out;
	}

	Var AST_Func::evaluate() const
	{
		return Var().pointerValue({ getID(), name });
	}

	bool AST_Func::run()
	{
		if (block()->addFunc(name, this))
		{
			return runNext();
		}
		return Debug::logError("AST_Func : Run Failed");
	}


	// Input
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Input::AST_Input()
		: AST_Expr(EX_Input)
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

		cout << "$: ";
		String line;
		std::getline(cin, line);
		return Var().stringValue(line);
#else
		return Var();
#endif
	}


	// Int
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Int::AST_Int(int32_t value)
		: AST_Expr(EX_Int)
		, value(value)
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
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Name::AST_Name(const String & value)
		: AST_Expr(EX_Name)
		, value(value)
	{
	}

	std::ostream & AST_Name::display(std::ostream & out) const
	{
		if (AST_Block * b = block())
		{
			if (Var * v = block()->getVar(value))
			{
				out << ( * v);
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
		if (AST_Block * b = block())
		{
			if (Var * v = block()->getVar(value))
			{
				return ( * v);
			}
			else
			{
				return * block()->setVar(value, Var().pointerValue({ block()->getID(), value }));
			}
		}

		return Var().errorValue("AST_Name : Null reference");
	}


	// BinOp
	/* * * * * * * * * * * * * * * * * * * * */
	AST_BinOp::AST_BinOp(const Operator & op, AST_Expr * lhs, AST_Expr * rhs)
		: AST_Expr(EX_BinOp)
		, op(op)
		, lhs(lhs)
		, rhs(rhs)
	{
		addChild(lhs);
		addChild(rhs);
	}

	std::ostream & AST_BinOp::display(std::ostream & out) const
	{
		out << "(" << * lhs << " " << op << " " << * rhs << ")";
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
			case OperatorType::OP_MOD:
				return lhs->evaluate() % rhs->evaluate();

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
		return Var().errorValue("AST_Oper : Invalid Operation {0} {1} {2}", * lhs, op, * rhs);
	}


	// String 
	/* * * * * * * * * * * * * * * * * * * * */
	AST_String::AST_String(const String & value)
		: AST_Expr(EX_String)
		, value(value)
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
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Struct::AST_Struct(const String & name, const Params & args)
		: AST_Expr(EX_Struct)
		, name(name)
		, args(args)
	{
		for (auto c : args)
			addChild(c);
	}

	AST_Block * AST_Struct::getBody() const
	{
		return nextAs<AST_Block>();
	}

	std::ostream & AST_Struct::display(std::ostream & out) const
	{
		out << (FG::DarkBlue | BG::Black) << name << " = $("
			<< FMT();
		Params::const_iterator it;
		for (it = args.begin(); it != args.end(); it++)
		{
			out << *(*it) << ((it != args.end() - 1) ? ", " : "");
		}
		out << (FG::DarkBlue | BG::Black) << ")"
			<< FMT();
		return out;
	}

	Var AST_Struct::evaluate() const
	{
		return Var().pointerValue({ getID(), name });
	}

	bool AST_Struct::run()
	{
		if (AST_Block * body = getBody())
		{
			if (block()->addStruct(name, this))
			{
				if (body->runFirst())
				{
					return runNext();
				}
				return Debug::logError("AST_Struct : Failed initializing body : {0}", name);
			}
			return Debug::logError("AST_Struct : Struct redefinition : {0}", name);
		}
		return Debug::logError("AST_Struct : Body not found : {0}", name);
	}


	// Subscript
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Subscr::AST_Subscr(AST_Name * name, AST_Expr * index)
		: AST_Expr(EX_Subscr)
		, name(name)
		, index(index)
	{
		addChild(name);
		addChild(index);
	}

	std::ostream & AST_Subscr::display(std::ostream & out) const
	{
		out << "" << name->value << "[" << * index << "]";
		return out;
	}

	Var AST_Subscr::evaluate() const
	{
		if (AST_Block * b = block())
		{
			if (Var * a = b->getVar(name->value))
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


	// Command
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Command::AST_Command(AST_Expr * expr)
		: AST_Expr(EX_Command)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_Command::display(std::ostream & out) const
	{
		return out << "command(" << ( * expr) << ")";
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
		return Debug::logError("AST_Sys : Call Failed");
	}


	// Size Of
	/* * * * * * * * * * * * * * * * * * * * */
	AST_SizeOf::AST_SizeOf(AST_Expr * expr)
		: AST_Expr(EX_SizeOf)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_SizeOf::display(std::ostream & out) const
	{
		return out << "sizeof(" << (*expr) << ")";
	}

	Var AST_SizeOf::evaluate() const
	{
		return Var().intValue(expr->evaluate().sizeOfValue());
	}


	// Type ID
	/* * * * * * * * * * * * * * * * * * * * */
	AST_TypeID::AST_TypeID(AST_Expr * expr)
		: AST_Expr(EX_TypeID)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_TypeID::display(std::ostream & out) const
	{
		return out << "typeid(" << (*expr) << ")";
	}

	Var AST_TypeID::evaluate() const
	{
		return Var().intValue(expr->evaluate().getTypeID());
	}


	// Type Name
	/* * * * * * * * * * * * * * * * * * * * */
	AST_TypeName::AST_TypeName(AST_Expr * expr)
		: AST_Expr(EX_TypeName)
		, expr(expr)
	{
		addChild(expr);
	}

	std::ostream & AST_TypeName::display(std::ostream & out) const
	{
		return out << "typename(" << (*expr) << ")";
	}

	Var AST_TypeName::evaluate() const
	{
		return Var().stringValue(expr->evaluate().getTypeName());
	}


	// Node ID
	/* * * * * * * * * * * * * * * * * * * * */
	AST_NodeID::AST_NodeID(AST_Expr * expr)
		: AST_Expr(EX_NodeID)
		, expr(expr)
	{
		addChild(expr);
	}
	
	std::ostream & AST_NodeID::display(std::ostream & out) const
	{
		return out << "nodeid(" << (*expr) << ")";
	}
	
	Var AST_NodeID::evaluate() const
	{
		return Var().intValue(expr->block()->getID());
	}


	// New
	/* * * * * * * * * * * * * * * * * * * * */
	AST_New::AST_New(AST_Expr * expr)
		: AST_Expr(EX_New)
		, expr(expr)
	{
		addChild(expr);
	}
	
	std::ostream & AST_New::display(std::ostream & out) const
	{
		return out << "new(" << (*expr) << ")";
	}
	
	Var AST_New::evaluate() const
	{
		if (expr)
		{
			if (AST_Name * name = expr->as<AST_Name>())
			{
				if (Var * v = block()->getVar(name->value))
				{
					if (v->isStructType())
					{
						if (AST_Struct * s = block()->getStruct(v->textValue()))
						{
							return s->evaluate();
						}
					}
				}
			}

			return expr->evaluate();
		}
		return Var().errorValue("AST_New : No Expression");
	}

	bool AST_New::run()
	{
		if (!evaluate().isErrorType())
		{
			return runNext();
		}
		return false;
	}


	// Member
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Member::AST_Member(AST_Name * name, AST_Name * expr)
		: AST_Expr(EX_Member)
		, name(name)
		, expr(expr)
	{
		addChild(name);
		addChild(expr);
	}
	
	AST_Struct * AST_Member::getStruct() const
	{
		if (name)
		{
			if (AST_Block * b = block())
			{
				if (Var * v = block()->getVar(name->value))
				{
					if (v->isStructType())
					{
						return b->getStruct(v->textValue());
					}
				}
			}
		}
		return NULL;
	}

	Var * AST_Member::getMember() const
	{
		if (expr)
		{
			if (AST_Struct * s = getStruct())
			{
				if (AST_Block * b = s->nextAs<AST_Block>())
				{
					if (Var * v = b->getVar(expr->value))
					{
						return v;
					}
				}
			}
		}
		return NULL;
	}
	
	std::ostream & AST_Member::display(std::ostream & out) const
	{
		return out 
			<< FG::Cyan  << "("
			<< (*name) 
			<< FG::Cyan << "." 
			<< (*expr)
			<< FG::Cyan << ")"
			<< FMT();
	}
	
	Var AST_Member::evaluate() const
	{
		if (Var * v = getMember())
		{
			return (*v);
		}
		return Var().errorValue("AST_Member : No Value");
	}
	
}