#include <MemeScript/Parser.h>
#include <MemeCore/StringUtility.h>
#include <MemeCore/DebugUtility.h>
#include <stack>

namespace ml
{
	Parser::Parser()
	{
		setFlags(Flags::None);
	}

	Parser::~Parser()
	{
	}



	Parser::ToksList Parser::SplitStatements(const TokenList & tokens)
	{
		static uint32_t id = 0;

		ToksList out = { TokenList() };

		uint32_t i = 0;

		TokenList::const_iterator it;
		for (it = tokens.begin(); it != tokens.end(); it++)
		{
			switch (it->type)
			{
			case '\n':
				continue;

			case '#':
				while (*it != Token::TOK_ENDL)
				{
					it++;
				}
				break;

			case '{':
				out[i].push_back(*it);
			case ';':
				if (!out[i].empty())
				{
					out.push_back(TokenList());
					i++;
				}
				break;

			default:
				out[i].push_back(*it);
				break;
			}
		}

		return out;
	}

	bool	Parser::InfixToPostfix(const TokenList & ifx, TokenList & pfx)
	{
		// just one operand
		if (ifx.size() == 1 && ifx.front().isOperand())
		{
			pfx.push_back(ifx.front());

			return true;
		}

		TokenList stk = { Token::TOK_LPRN };

		pfx = TokenList();

		for (uint32_t i = 0; i < ifx.size(); i++)
		{
			//out() << pfx << std::endl;

			const Token& arg = ifx[i];

			if (arg.isOperator())
			{
				uint32_t count = 0;

				while (!stk.empty() && (stk.front() <= arg))
				{
					pfx.push_back(stk.front());

					stk.erase(stk.begin());

					if (++count > 100)
					{
						Debug::LogError("Stack overflow");

						return false;
					}
				}

				stk.insert(stk.begin(), arg);
			}
			else if (arg == '(')
			{
				stk.insert(stk.begin(), arg);
			}
			else if (arg == ')')
			{
				while (!stk.empty())
				{
					if (stk.front() == '(')
						break;

					pfx.push_back(stk.front());

					stk.erase(stk.begin());
				}

				if (!stk.empty())
				{
					stk.erase(stk.begin());
				}

				if (stk.empty())
				{
					Debug::LogError("Missing left parenthesis (1)\n");
					return false;
				}
			}
			else if (arg.isOperand())
			{
				pfx.push_back(arg);
			}
		}

		while (!stk.empty() && stk.front() != Token::TOK_LPRN)
		{
			pfx.push_back(stk.front());

			stk.erase(stk.begin());
		}

		if (stk.empty())
		{
			Debug::LogError("Missing left parenthesis (2)\n");
			return false;
		}

		stk.erase(stk.begin());

		//if (!stk.empty())
		//{
		//	Debug::LogError("Missing left parenthesis (3)\n");
		//	out() << stk << std::endl;
		//	return false;
		//}

		// Final Error Checking
		uint32_t numOperators = 0;
		uint32_t numOperands = 0;

		for (uint32_t i = 0; i < pfx.size(); i++)
		{
			const Token& arg = pfx[i];

			if (arg.isOperator())
			{
				numOperators++;
			}
			else
			{
				numOperands++;
			}
		}

		if (numOperators == 0 && numOperands == 1)
		{
			return true;
		}
		else if (numOperands <= 1)
		{
			Debug::LogError("Not enough operands");
			return false;
		}
		else if (numOperators >= numOperands)
		{
			Debug::LogError("Too many operators");
			return false;
		}

		return true;
	}

	bool	Parser::MakeOperator(const Token & lhs, const Token & rhs, Operator & op)
	{
		return Operator::makeOperator(lhs.data + rhs.data, op);
	}


	// Upper Level

	AST_Block*	Parser::genAST(const TokenList& tokens) const
	{
		AST_Block* root = NULL;

		if (!tokens.empty())
		{
			Parser::ToksList statements = SplitStatements(tokens);
			if (!statements.empty())
			{
				root = new AST_Block({});

				ToksList::const_iterator it;
				for (it = statements.begin(); it != statements.end(); it++)
				{
					if (getFlags() & Parser::ShowToks)
					{
						std::cerr << std::endl << (*it) << std::endl;
					}

					// For
					if (it->matchD(it->begin(), { "for" }))
					{
						TokenList args = TokenList(*it).after(2);
						if (AST_Assign* assn = genAssign(args))
						{
							args = TokenList((*++it));

							if (AST_Expr* expr = genComplex(args))
							{
								args = TokenList((*++it)).pop_back();

								if (AST_Stmt* stmt = genStatement(args))
								{
									if (root->addChild(new AST_For(assn, expr, stmt)))
									{
										continue;
									}
									delete stmt;
								}
								delete expr;
							}
							delete assn;
						}
					}
					else if (AST_Node* node = genNode(root, (*it)))
					{
						if (node == root)
						{
							root = root->block();
						}
						else
						{
							root->addChild(node);

							if (AST_Block* blck = node->As<AST_Block>())
							{
								root = blck;
							}
						}
					}
				}

				if (getFlags() & Flags::ShowTree)
				{
					std::cerr << std::endl << (*root) << std::endl;
				}
			}
		}

		return root;
	}

	AST_Node*	Parser::genNode(AST_Node* root, const TokenList & toks) const
	{
		if (toks.empty())
			return NULL;

		for (TokenList::const_iterator it = toks.begin(); it != toks.end(); it++)
		{
			switch (it->type)
			{
			case '{':
				return new AST_Block({});

			case '}':
				if (AST_Node* parent = root->getParent())
				{
					return root;
				}

			default:
				if (AST_Stmt * stmt = genStatement(toks))
				{
					return stmt;
				}
			}
		}

		return NULL;
	}

	AST_Stmt*	Parser::genStatement(const TokenList & toks) const
	{
		// If
		if (toks.matchD(toks.begin(), { "if" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_If(expr);
			}
		}
		// Elif
		else if (toks.matchD(toks.begin(), { "elif" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_Elif(expr);
			}
		}
		// Else
		else if (toks.matchD(toks.begin(), { "else" }))
		{
			return new AST_Else();
		}
		// Print
		else if (toks.matchD(toks.begin(), { "print" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			switch (params.size())
			{
			case 1:
				return new AST_Print(params.front());
			case 0:
			default:
				return new AST_Print(new AST_Str(std::string()));
			}
		}
		// Return
		else if (toks.matchD(toks.begin(), { "return" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_Return(expr);
			}
			else
			{
				return new AST_Return(new AST_Int(0));
			}
		}
		// While
		else if (toks.matchD(toks.begin(), { "while" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_While(expr);
			}
		}
		// Free
		else if (toks.matchD(toks.begin(), { "free" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				if (AST_Name* name = expr->As<AST_Name>())
				{
					return new AST_Free(name);
				}
				else
				{
					delete expr;
					return NULL;
				}
			}
		}
		// If
		else if (toks.matchD(toks.begin(), { "include" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				if (AST_Str* str = expr->As<AST_Str>())
				{
					return new AST_Include(str);
				}
				delete expr;
			}
		}

		return genComplex(toks);
	}

	AST_Expr*	Parser::genComplex(const TokenList & toks) const
	{
		if (toks.empty())
		{
			return NULL;
		}
		else if (toks.size() == 1)
		{
			return genSimple(toks.front());
		}
		else if (toks.isWrap('(', ')'))
		{
			return genComplex(toks.unwrapped());
		}
		else if (toks.size() == 4 && toks.matchS(toks.begin(), "EOOE"))
		{
			Operator op;
			if (MakeOperator(toks[1], toks[2], op))
			{
				return new AST_Oper(op, genSimple(toks.front()), genSimple(toks.back()));
			}
		}
		// System
		else if (toks.matchD(toks.begin(), { "sys" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				if (AST_Str* str = expr->As<AST_Str>())
				{
					return new AST_Sys(str);
				}
			}
		}
		// Function
		else if (AST_Func* func = genFunc(toks))
		{
			return func;
		}
		// Assignment
		else if (AST_Assign* assign = genAssign(toks))
		{
			return assign;
		}
		// Input
		else if (AST_Input* input = genInput(toks))
		{
			return input;
		}
		// Call
		else if (AST_Call* call = genCall(toks))
		{
			return call;
		}
		// Subscript
		else if (AST_Subscr* sub = genSubscr(toks))
		{
			return sub;
		}
		// Array
		else if (AST_Array* arr = genArray(toks))
		{
			return arr;
		}

		TokenList ifx(toks), pfx;
		if (InfixToPostfix(ifx, pfx))
		{
			if (AST_Oper* oper = genOper(pfx))
			{
				return oper;
			}
			else if (AST_Expr* expr = genComplex(pfx))
			{
				return expr;
			}
			else
			{
				return new AST_Str(pfx.str());
			}
		}

		return NULL;
	}

	AST_Expr*	Parser::genSimple(const Token & token) const
	{
		switch (token.type)
		{
		case Token::TOK_FLT:
			return new AST_Flt(StringUtility::ToFloat(token.data));

		case Token::TOK_INT:
			return new AST_Int(StringUtility::ToInt(token.data));

		case Token::TOK_STR:
			return new AST_Str(token.data);

		case Token::TOK_NAME:
			if (StringUtility::IsBool(token.data))
			{
				return new AST_Bool(StringUtility::ToBool(token.data));
			}
			return new AST_Name(token.data);

		default:
			return NULL;
		}
	}


	// Simple Expressions

	AST_Bool*	Parser::genBool(const Token & token) const
	{
		if (token == Token::TOK_NAME)
		{
			if (StringUtility::IsBool(token.data))
			{
				return new AST_Bool(StringUtility::ToBool(token.data));
			}
		}
		return NULL;
	}

	AST_Flt*	Parser::genFlt(const Token & token) const
	{
		if (token == Token::TOK_FLT)
		{
			if (StringUtility::IsDecimal(token.data))
			{
				return new AST_Flt(StringUtility::ToFloat(token.data));
			}
		}
		return NULL;
	}

	AST_Int*	Parser::genInt(const Token & token) const
	{
		if (token == Token::TOK_INT)
		{
			if (StringUtility::IsInt(token.data))
			{
				return new AST_Int(StringUtility::ToInt(token.data));
			}
		}
		return NULL;
	}

	AST_Name*	Parser::genName(const Token & token) const
	{
		if (token == Token::TOK_NAME)
		{
			return new AST_Name(token.data);
		}
		return NULL;
	}

	AST_Str*	Parser::genStr(const Token & token) const
	{
		if (token == Token::TOK_STR)
		{
			return new AST_Str(token.data);
		}
		return NULL;
	}


	// Complex Expressions

	AST_Array*	Parser::genArray(const TokenList & toks) const
	{
		if (toks.isWrap('[', ']'))
		{
			if (toks.matchS(toks.begin(), "[]"))
			{
				return new AST_Array({});
			}
			else
			{
				return new AST_Array(genArrayElements(toks.unwrapped()));
			}
		}

		return NULL;
	}

	AST_Assign* Parser::genAssign(const TokenList & toks) const
	{
		const TokenList::const_iterator& it = toks.begin();

		if (toks.matchS(it, "n=A"))
		{
			return new AST_Assign(
				Operator::OP_SET,
				new AST_Name(it->data),
				genComplex(toks.after(2)));
		}
		else if (toks.matchS(it, "nO=A"))
		{
			Operator op;
			if (MakeOperator(*(it + 1), *(it + 2), op))
			{
				return new AST_Assign(
					op,
					new AST_Name(it->data),
					genComplex(toks.after(3)));
			}
		}
		return NULL;
	}

	AST_Call*	Parser::genCall(const TokenList & toks) const
	{
		if (toks.matchS(toks.begin(), "n(") && toks.back(')'))
		{
			return new AST_Call(
				new AST_Name(toks.front().data),
				genCallParams(toks.after(1)));
		}
		return NULL;
	}

	AST_Func*	Parser::genFunc(const TokenList & toks) const
	{
		if (toks.matchS(toks.begin(), "n=[](") && toks.back(')'))
		{
			return new AST_Func(
				toks.front().data,
				genFuncParams(toks.after(4)));
		}
		return NULL;
	}

	AST_Input * Parser::genInput(const TokenList & toks) const
	{
		if (toks.matchS(toks.begin(), "n(") && toks.back(')'))
		{
			if (toks.front("input"))
			{
				// new AST_Name(toks.front().data)
				// genCallParams(toks.after(1))
				return new AST_Input();
			}
		}
		return NULL;
	}

	AST_Oper*	Parser::genOper(const TokenList & toks) const
	{
		std::stack<AST_Expr*> q;

		TokenList::const_iterator it;
		for (it = toks.begin(); it != toks.end(); it++)
		{
			Operator op;
			if (Operator::makeOperator(it->data, op))
			{
				if (q.size() < 2)
				{
					return NULL;
				}

				AST_Expr* rhs = q.top();
				q.pop();

				AST_Expr* lhs = q.top();
				q.pop();

				q.push(new AST_Oper(op, lhs, rhs));
			}
			else
			{
				q.push(genSimple(*it));
			}
		}

		if (AST_Oper* oper = dynamic_cast<AST_Oper*>(q.top()))
		{
			return oper;
		}

		return NULL;
	}

	AST_Subscr* Parser::genSubscr(const TokenList & toks) const
	{
		if (toks.matchS(toks.begin(), "n[E]"))
		{
			if (AST_Name* name = new AST_Name(toks.front().data))
			{
				if (AST_Expr* expr = genSimple(toks[2]))
				{
					return new AST_Subscr(name, expr);
				}
			}
		}
		return NULL;
	}


	// Expression Containers

	AST_Array::Values	Parser::genArrayElements(const TokenList & toks) const
	{
		AST_Array::Values elems;

		TokenList	arg;
		int		depth = 0;

		for (TokenList::const_iterator it = toks.begin(); it != toks.end(); it++)
		{
			if (*it == '(')
			{
				depth++;

				arg.push_back(*it);
			}
			else if (*it == ')')
			{
				if (--depth > 0)
				{
					arg.push_back(*it);
				}
			}
			else if (*it == ',' && !depth)
			{
				if (!arg.empty())
				{
					elems.push_back(genComplex(arg));

					arg.clear();

					continue;
				}

				arg.push_back(*it);
			}
			else
			{
				arg.push_back(*it);
			}
		}

		if (!arg.empty())
		{
			elems.push_back(genComplex(arg));
		}

		return elems;
	}

	AST_Call::Params	Parser::genCallParams(const TokenList & toks) const
	{
		AST_Call::Params params;

		if (toks.matchS(toks.begin(), "()"))
		{
			return params;
		}

		TokenList	arg;
		int		depth = 0;

		for (TokenList::const_iterator it = toks.begin() + 1; it != toks.end(); it++)
		{
			if (*it == '(')
			{
				depth++;

				arg.push_back(*it);
			}
			else if (*it == ')')
			{
				if (--depth > 0)
				{
					arg.push_back(*it);
				}
			}
			else if (*it == ',' && !depth)
			{
				if (!arg.empty())
				{
					params.push_back(genComplex(arg));
					arg.clear();
					continue;
				}

				arg.push_back(*it);
			}
			else
			{
				arg.push_back(*it);
			}
		}

		if (!arg.empty())
		{
			params.push_back(genComplex(arg));
		}

		return params;
	}

	AST_Func::Params	Parser::genFuncParams(const TokenList & toks) const
	{
		AST_Func::Params params;

		if (toks.isWrap('(', ')'))
		{
			for (TokenList::const_iterator it = toks.begin() + 1; it != toks.end() - 1; it++)
			{
				if ((*it).type != Token::TOK_CMMA)
				{
					params.push_back(new AST_Name(it->data));
				}
			}
		}

		return params;
	}

}