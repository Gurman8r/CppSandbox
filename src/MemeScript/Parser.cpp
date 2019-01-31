#include <MemeScript/Parser.h>
#include <MemeCore/StringUtility.h>
#include <MemeCore/Debug.h>
#include <stack>

namespace ml
{
	Parser::ToksList Parser::SplitStatements(const TokenList & tokens)
	{
		static size_t id = 0;

		ToksList out = { TokenList() };

		size_t i = 0;

		TokenList::const_iterator it;
		for (it = tokens.begin(); it != tokens.end(); it++)
		{
			switch (it->type)
			{
			case '\n':
				continue;

			case '#':
			{
				string line;
				while (*it != TokenType::TOK_ENDL)
				{
					line += (*it++).data;
				}
			}
			break;

			case '{':
			{
				out[i].push_back(*it);
			}
			case ';':
			{
				if (!out[i].empty())
				{
					out.push_back(TokenList());
					i++;
				}
			}
			break;

			default:
			{
				out[i].push_back(*it);
			}
			break;
			}
		}

		return out;
	}

	bool	Parser::InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show)
	{
		// just one operand
		if (ifx.size() == 1 && ifx.front().isOperand())
		{
			pfx.push_back(ifx.front());

			return true;
		}

		TokenList stk = { TokenType::TOK_LPRN };

		pfx = TokenList();

		bool func = false;
		for (auto it = ifx.cbegin(); it != ifx.cend(); it++)
		{
			const Token& arg = (*it);

			if (arg.isOperator())
			{
				size_t count = 0;

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
			else if (arg == TokenType::TOK_LPRN) // (
			{
				TokenList::const_iterator prev = (it - 1);
				if (!func && ((prev >= ifx.begin()) && prev->type == TokenType::TOK_NAME))
				{
					pfx.push_back(arg);
					func = true;
				}
				else
				{
					stk.insert(stk.begin(), arg);
				}
			}
			else if (arg == TokenType::TOK_RPRN) // )
			{
				size_t count = 0;
				while (!stk.empty())
				{
					if (stk.front() == TokenType::TOK_LPRN)
					{
						break;
					}
					pfx.push_back(stk.front());
					stk.erase(stk.begin());
					count++;
				}

				if (func)
				{
					func = false;
					pfx.insert(pfx.end() - count, arg);
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

			if(show) cout << "P: " << pfx << std::endl;
		}

		while (!stk.empty() && stk.front() != TokenType::TOK_LPRN)
		{
			pfx.push_back(stk.front());
			stk.erase(stk.begin());
		}

		if (show) cout << "P: " << pfx << std::endl;

		if (stk.empty())
		{
			Debug::LogError("Missing left parenthesis (2)\n");
			return false;
		}

		stk.erase(stk.begin());

		// Final Error Checking
		size_t numOperators = 0;
		size_t numOperands = 0;

		for (size_t i = 0; i < pfx.size(); i++)
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
					if (m_showToks)
					{
						cout << (*it) << std::endl;
					}

					// For
					if (it->matchData(it->begin(), { "for" }))
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

				if (m_showTree)
				{
					cout << (*root) << std::endl;
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
		if (toks.empty())
		{
			return NULL;
		}
		// If
		else if (toks.matchData(toks.begin(), { "if" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_If(expr);
			}
		}
		// Elif
		else if (toks.matchData(toks.begin(), { "elif" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_Elif(expr);
			}
		}
		// Else
		else if (toks.matchData(toks.begin(), { "else" }))
		{
			return new AST_Else();
		}
		// Printl
		else if (toks.matchData(toks.begin(), { "printl" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			switch (params.size())
			{
			case 1:	 return new AST_Print(params.front(), true);
			default: return new AST_Print(new AST_String(string()), true);
			}
		}
		// Print
		else if (toks.matchData(toks.begin(), { "print" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			switch (params.size())
			{
			case 1:	 return new AST_Print(params.front(), false);
			default: return new AST_Print(new AST_String(string()), false);
			}
		}
		// Return
		else if (toks.matchData(toks.begin(), { "return" }))
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
		else if (toks.matchData(toks.begin(), { "while" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				return new AST_While(expr);
			}
		}
		// Delete
		else if (toks.matchData(toks.begin(), { "delete" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				if (AST_Name* name = expr->As<AST_Name>())
				{
					return new AST_Delete(name);
				}
				else
				{
					delete expr;
					return NULL;
				}
			}
		}
		// Include
		else if (toks.matchData(toks.begin(), { "include" }))
		{
			if (AST_Expr* expr = genComplex(toks.between('(', ')').pop_front()))
			{
				if (AST_String* str = expr->As<AST_String>())
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
		else if (toks.size() == 4 && toks.matchStr(toks.begin(), "EOOE"))
		{
			Operator op;
			if (MakeOperator(toks[1], toks[2], op))
			{
				return new AST_BinOp(op, genSimple(toks.front()), genSimple(toks.back()));
			}
		}
		// Command
		else if (toks.matchData(toks.begin(), { "command" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			if (!params.empty())
			{
				return new AST_Command(params.front());
			}
			return new AST_TypeID(new AST_Int(0));
		}
		// Size Of
		else if (toks.matchData(toks.begin(), { "sizeof" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			if (!params.empty())
			{
				return new AST_SizeOf(params.front());
			}
			return new AST_TypeID(new AST_Int(0));
		}
		// Type ID
		else if (toks.matchData(toks.begin(), { "typeid" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			if (!params.empty())
			{
				return new AST_TypeID(params.front());
			}
			return new AST_TypeID(new AST_Int(0));
		}
		// Type Name
		else if (toks.matchData(toks.begin(), { "typename" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			if (!params.empty())
			{
				return new AST_TypeName(params.front());
			}
			return new AST_TypeName(new AST_Int(0));
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
		if (InfixToPostfix(ifx, pfx, m_showItoP))
		{
			if (AST_BinOp* oper = genBinOp(pfx))
			{
				return oper;
			}
			else if (AST_Expr* expr = genComplex(pfx))
			{
				return expr;
			}
			else
			{
				return new AST_String(pfx.str());
			}
		}

		return NULL;
	}

	AST_Expr*	Parser::genSimple(const Token & token) const
	{
		switch (token.type)
		{
		case TokenType::TOK_FLT:
			return new AST_Flt(StringUtility::ToFloat(token.data));

		case TokenType::TOK_INT:
			return new AST_Int(StringUtility::ToInt(token.data));

		case TokenType::TOK_STR:
			return new AST_String(token.data);

		case TokenType::TOK_NAME:
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
		if (token == TokenType::TOK_NAME)
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
		if (token == TokenType::TOK_FLT)
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
		if (token == TokenType::TOK_INT)
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
		if (token == TokenType::TOK_NAME)
		{
			return new AST_Name(token.data);
		}
		return NULL;
	}

	AST_String*	Parser::genStr(const Token & token) const
	{
		if (token == TokenType::TOK_STR)
		{
			return new AST_String(token.data);
		}
		return NULL;
	}


	// Complex Expressions

	AST_Array*	Parser::genArray(const TokenList & toks) const
	{
		if (toks.isWrap('[', ']'))
		{
			if (toks.matchStr(toks.begin(), "[]"))
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
		TokenList::const_iterator it = toks.begin();

		if (toks.matchStr(it, "n[E]=A"))
		{
			TokenList list({ *(it), *(it + 1), *(it + 2), *(it + 3) });

			if (AST_Subscr* subscr = genSubscr(list))
			{
				if (AST_Expr* value = genComplex(toks.after(5)))
				{
					return NULL;
					//return new AST_Assign(OperatorType::OP_SET, subscr, value);
				}
			}
		}
		else if (toks.matchStr(it, "n[E]O=A"))
		{
			TokenList list({ *(it), *(it + 1), *(it + 2), *(it + 3) });

			if (AST_Subscr* subscr = genSubscr(list))
			{
				Operator op;
				if (MakeOperator(*(it + 4), *(it + 5), op))
				{
					if (AST_Expr* value = genComplex(toks.after(6)))
					{
						return NULL;
						//return new AST_Assign(op, subscr, value);
					}
				}
			}
		}
		else if (toks.matchStr(it, "n=A"))
		{
			return new AST_Assign(
				OperatorType::OP_SET,
				new AST_Name(it->data),
				genComplex(toks.after(2)));
		}
		else if (toks.matchStr(it, "nO=A"))
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
		if (toks.matchStr(toks.begin(), "n(") && toks.back(")"))
		{
			return new AST_Call(
				new AST_Name(toks.front().data),
				genCallParams(toks.after(1)));
		}
		return NULL;
	}

	AST_Func*	Parser::genFunc(const TokenList & toks) const
	{
		if (toks.matchStr(toks.begin(), "n=[](") && toks.back(')'))
		{
			return new AST_Func(
				toks.front().data,
				genFuncParams(toks.after(4)));
		}
		return NULL;
	}

	AST_Input * Parser::genInput(const TokenList & toks) const
	{
		if (toks.matchStr(toks.begin(), "n(") && toks.back(')'))
		{
			if (toks.front("input"))
			{
				return new AST_Input();
			}
		}
		return NULL;
	}

	AST_BinOp*	Parser::genBinOp(const TokenList & toks) const
	{
		std::stack<AST_Expr*> stk;

		TokenList::const_iterator it;
		for (it = toks.begin(); it != toks.end(); it++)
		{
			// Call
			if ((it)->type == TOK_NAME && (it + 1)->type == TOK_LPRN)
			{
				TokenList call;
				call.push_back(*it);
				while ((it++)->type != TOK_RPRN)
				{
					call.push_back(*it);
				}
				//cout << "Call: " << call << std::Endl;
				stk.push(genCall(call));
			}

			Operator op;
			if (Operator::makeOperator(it->data, op))
			{
				if (stk.size() < 2)
				{
					return NULL;
				}

				AST_Expr* rhs = stk.top();
				stk.pop();
				AST_Expr* lhs = stk.top();
				stk.pop();
				stk.push(new AST_BinOp(op, lhs, rhs));
			}
			else
			{
				stk.push(genComplex(*it));
			}
		}

		if (AST_BinOp* oper = stk.top()->As<AST_BinOp>())
		{
			return oper;
		}

		return NULL;
	}

	AST_Subscr* Parser::genSubscr(const TokenList & toks) const
	{
		if (toks.matchStr(toks.begin(), "n[E]"))
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
		int32_t		depth = 0;

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

		if (toks.matchStr(toks.begin(), "()"))
		{
			return params;
		}

		TokenList	arg;
		int32_t		depth = 0;

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
				if ((*it).type != TokenType::TOK_CMMA)
				{
					params.push_back(new AST_Name(it->data));
				}
			}
		}

		return params;
	}

}