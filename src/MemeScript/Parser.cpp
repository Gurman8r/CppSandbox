#include <MemeScript/Parser.hpp>
#include <MemeScript/StringUtility.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/Function.hpp>
#include <MemeScript/Interpreter.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using SyntaxTree = typename Parser::SyntaxTree;
	using RuleMap	 = typename Parser::RuleMap;

	/* * * * * * * * * * * * * * * * * * * * */

	Parser::Parser()
	{
		install_expressions();
	}

	Parser::~Parser()
	{
		for (auto pair : m_rules)
		{
			delete pair.second;
			pair.second = NULL;
		}
		m_rules.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Parser::install_expressions()
	{
		addRule<AST_Bool>(new NodeMaker<AST_Bool>([](const TokenList & toks)
		{
			AST_Bool * temp;
			return (
				(toks.size() == 1) &&
				(toks.front(TokenType::TOK_NAME)) &&
				(StringUtility::IsBool(toks.front().data))
					? (temp = new AST_Bool(StringUtility::ToBool(toks.front().data)))
					: (temp = NULL)
				);
		}));

		addRule<AST_Int>(new NodeMaker<AST_Int>([](const TokenList & toks)
		{
			AST_Int * temp;
			return (
				(toks.size() == 1) &&
				(toks.front(TokenType::TOK_INT)) &&
				(StringUtility::IsInt(toks.front().data))
					? (temp = new AST_Int(StringUtility::ToInt(toks.front().data)))
					: (temp = NULL)
				);
		}));

		addRule<AST_Float>(new NodeMaker<AST_Float>([](const TokenList & toks)
		{
			AST_Float * temp;
			return (
				(toks.size() == 1) &&
				(toks.front(TokenType::TOK_FLT)) &&
				(StringUtility::IsDecimal(toks.front().data))
					? (temp = new AST_Float(StringUtility::ToFloat(toks.front().data)))
					: (temp = NULL)
				);
		}));

		addRule<AST_Name>(new NodeMaker<AST_Name>([](const TokenList & toks)
		{
			AST_Name * temp;
			return (
				((toks.size() == 1) && (toks.front(TokenType::TOK_NAME)))
					? (temp = new AST_Name(toks.front().data))
					: (temp = NULL)
				);
		}));

		addRule<AST_String>(new NodeMaker<AST_String>([](const TokenList & toks)
		{
			AST_String * temp;
			return (
				((toks.size() == 1) && (toks.front(TokenType::TOK_STR)))
					? (temp = new AST_String(toks.front().data))
					: (temp = NULL)
				);
		}));

		addRule<AST_Member>(new NodeMaker<AST_Member>([](const TokenList & toks)
		{
			AST_Member * temp;
			if (toks.matchStr(toks.begin(), "n.n"))
			{
				if (AST_Name * n = ML_Parser.generate<AST_Name>(toks.front()))
				{
					if (AST_Name * e = ML_Parser.generate<AST_Name>(toks.back()))
					{
						return (temp = new AST_Member(n, e));
					}
					else { delete e; }
				}
				else { delete n; }
			}
			return (temp = NULL);
		}));

		addRule<AST_Struct>(new NodeMaker<AST_Struct>([](const TokenList & toks)
		{
			AST_Struct * temp;
			if (toks.matchStr(toks.begin(), "n=$(") && toks.back(')'))
			{
				return (temp = new AST_Struct(
					toks.front().data,
					ML_Parser.genFuncParams(toks.after(3))
				));
			}
			return (temp = NULL);
		}));

		addRule<AST_Command>(new NodeMaker<AST_Command>([](const TokenList & toks)
		{
			AST_Command * temp;
			if (toks.matchData(toks.begin(), { "command" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1:	return (temp = new AST_Command(params.front()));
				default: return (temp = new AST_Command(new AST_String(String())));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_New>(new NodeMaker<AST_New>([](const TokenList & toks)
		{
			AST_New * temp;
			if (toks.matchData(toks.begin(), { "new" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1:	return (temp = new AST_New(params.front()));
				default: return (temp = new AST_New(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_NodeID>(new NodeMaker<AST_NodeID>([](const TokenList & toks)
		{
			AST_NodeID * temp;
			if (toks.matchData(toks.begin(), { "nodeid" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1: return (temp = new AST_NodeID(params.front()));
				default: return (temp = new AST_NodeID(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_SizeOf>(new NodeMaker<AST_SizeOf>([](const TokenList & toks)
		{
			AST_SizeOf * temp;
			if (toks.matchData(toks.begin(), { "sizeof" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1: return (temp = new AST_SizeOf(params.front()));
				default: return (temp = new AST_SizeOf(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_TypeID>(new NodeMaker<AST_TypeID>([](const TokenList & toks)
		{
			AST_TypeID * temp;
			if (toks.matchData(toks.begin(), { "typeid" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1: return (temp = new AST_TypeID(params.front()));
				default: return (temp = new AST_TypeID(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_TypeName>(new NodeMaker<AST_TypeName>([](const TokenList & toks)
		{
			AST_TypeName * temp;
			if (toks.matchData(toks.begin(), { "typename" }))
			{
				AST_Call::Params params = ML_Parser.genCallParams(toks.after(1));
				switch (params.size())
				{
				case 1: return (temp = new AST_TypeName(params.front()));
				default: return (temp = new AST_TypeName(new AST_Int(0)));
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_Call>(new NodeMaker<AST_Call>([](const TokenList & toks)
		{
			AST_Call * temp;
			if (toks.matchStr(toks.begin(), "n(") && toks.back(")"))
			{
				return (temp = new AST_Call(
					new AST_Name(toks.front().data),
					ML_Parser.genCallParams(toks.after(1))
				));
			}
			return (temp = NULL);
		}));

		addRule<AST_Func>(new NodeMaker<AST_Func>([](const TokenList & toks)
		{
			AST_Func * temp;
			if (toks.matchStr(toks.begin(), "n=[](") && toks.back(')'))
			{
				return (temp = new AST_Func(
					toks.front().data,
					ML_Parser.genFuncParams(toks.after(4))
				));
			}
			return (temp = NULL);
		}));

		addRule<AST_Input>(new NodeMaker<AST_Input>([](const TokenList & toks)
		{
			AST_Input * temp;
			if (toks.matchStr(toks.begin(), "n(") && toks.back(')'))
			{
				if (toks.front("input"))
				{
					return (temp = new AST_Input());
				}
			}
			return (temp = NULL);
		}));

		addRule<AST_Subscr>(new NodeMaker<AST_Subscr>([](const TokenList & toks)
		{
			AST_Subscr * temp;
			if (toks.matchStr(toks.begin(), "n[E]"))
			{
				if (AST_Name * n = new AST_Name(toks.front().data))
				{
					if (AST_Expr * e = ML_Parser.genExpression(toks[2]))
					{
						return (temp = new AST_Subscr(n, e));
					}
					else { delete e; }
				}
				else { delete n; }
			}
			return (temp = NULL);
		}));

		addRule<AST_Array>(new NodeMaker<AST_Array>([](const TokenList & toks)
		{
			AST_Array * temp;
			return ((toks.isWrap('[', ']'))
				? ((toks.matchStr(toks.begin(), "[]"))
					? (temp = new AST_Array({}))
					: (temp = new AST_Array(ML_Parser.genArrayElements(toks.unwrapped()))))
				: (temp = NULL));
		}));

	}

	/* * * * * * * * * * * * * * * * * * * * */

	SyntaxTree Parser::SplitStatements(const TokenList & tokens)
	{
		static size_t id = 0;

		SyntaxTree out = { TokenList() };

		size_t i = 0;

		TokenList::const_iterator it;
		for (it = tokens.begin(); it != tokens.end(); it++)
		{
			switch (it->type)
			{
			case '\n': // New Line
				continue;

			case '#': // Comment
			{
				String line;
				while ((it != tokens.end()) && ((*it) != TokenType::TOK_ENDL))
				{
					line += (*it++).data;
				}
			}
			break;

			case '{': // Begin Block
				out[i].push_back(*it);

			case ';': // End Statement
				if (!out[i].empty())
				{
					out.push_back(TokenList());
					i++;
				}
				break;

			default: // 
				out[i].push_back(*it);
				break;
			}
		}

		return out;
	}

	bool Parser::InfixToPostfix(const TokenList & ifx, TokenList & pfx, bool show)
	{
		// just one operand
		if (ifx.size() == 1 && ifx.front().isOperand())
		{
			pfx.push_back(ifx.front());

			return true;
		}

		TokenList stack = { TokenType::TOK_LPRN };

		pfx = TokenList();

		bool func = false;
		for (auto it = ifx.cbegin(); it != ifx.cend(); it++)
		{
			const Token & arg = (*it);

			if (arg.isOperator())
			{
				size_t count = 0;

				while (!stack.empty() && (stack.front() <= arg))
				{
					pfx.push_back(stack.front());

					stack.erase(stack.begin());

					if (++count > 100)
					{
						return Debug::logError("Stack overflow");
					}
				}

				stack.insert(stack.begin(), arg);
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
					stack.insert(stack.begin(), arg);
				}
			}
			else if (arg == TokenType::TOK_RPRN) // )
			{
				size_t count = 0;
				while (!stack.empty())
				{
					if (stack.front() == TokenType::TOK_LPRN)
					{
						break;
					}
					pfx.push_back(stack.front());
					stack.erase(stack.begin());
					count++;
				}

				if (func)
				{
					func = false;
					pfx.insert(pfx.end() - count, arg);
				}

				if (!stack.empty())
				{
					stack.erase(stack.begin());
				}

				if (stack.empty())
				{
					Debug::logError("Missing left parenthesis (1)\n");
					return false;
				}
			}
			else if (arg.isOperand())
			{
				pfx.push_back(arg);
			}

			if (show) cout << "P: " << pfx << ml::endl;
		}

		while (!stack.empty() && stack.front() != TokenType::TOK_LPRN)
		{
			pfx.push_back(stack.front());
			stack.erase(stack.begin());
		}

		if (show) cout << "P: " << pfx << ml::endl;

		if (stack.empty())
		{
			return Debug::logError("Missing left parenthesis (2)\n");
		}

		stack.erase(stack.begin());

		// Final Error Checking
		size_t numOperators = 0;
		size_t numOperands = 0;

		for (size_t i = 0; i < pfx.size(); i++)
		{
			const Token & arg = pfx[i];

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
			return Debug::logError("Not enough operands");
		}
		else if (numOperators >= numOperands)
		{
			return Debug::logError("Too many operators");
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	AST_Block * Parser::genAST(const TokenList & tokens) const
	{
		AST_Block * root = NULL;

		if (!tokens.empty())
		{
			Parser::SyntaxTree statements = SplitStatements(tokens);
			if (!statements.empty())
			{
				root = new AST_Block({});

				SyntaxTree::const_iterator toks;
				for (toks = statements.begin(); toks != statements.end(); toks++)
				{
					if (m_showToks)
					{
						cout << (*toks) << ml::endl;
					}

					// For
					if (toks->matchData(toks->begin(), { "for" }))
					{
						TokenList args = TokenList(*toks).after(2);
						if (AST_Assign * assn = genAssign(args))
						{
							args = TokenList((*++toks));

							if (AST_Expr * expr = genExpression(args))
							{
								args = TokenList((*++toks)).pop_back();

								if (AST_Stmt * stmt = genStatement(args))
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
					else if (AST_Node * node = genNode(root, (*toks)))
					{
						if (node == root)
						{
							root = root->block();
						}
						else
						{
							root->addChild(node);

							if (AST_Block * blck = node->Cast<AST_Block>())
							{
								root = blck;
							}
						}
					}
				}

				if (m_showTree)
				{
					cout << (*root) << ml::endl;
				}
			}
		}

		return root;
	}

	AST_Node * Parser::genNode(AST_Node* root, const TokenList & toks) const
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
				if (AST_Node * parent = root->getParent())
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

	AST_Stmt * Parser::genStatement(const TokenList & toks) const
	{
		if (toks.empty())
		{
			return NULL;
		}
		// If
		else if (toks.matchData(toks.begin(), { "if" }))
		{
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
			{
				return new AST_If(expr);
			}
		}
		// Elif
		else if (toks.matchData(toks.begin(), { "elif" }))
		{
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
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
			default: return new AST_Print(new AST_String(String()), true);
			}
		}
		// Print
		else if (toks.matchData(toks.begin(), { "print" }))
		{
			AST_Call::Params params = genCallParams(toks.after(1));
			switch (params.size())
			{
			case 1:	 return new AST_Print(params.front(), false);
			default: return new AST_Print(new AST_String(String()), false);
			}
		}
		// Return
		else if (toks.matchData(toks.begin(), { "return" }))
		{
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
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
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
			{
				return new AST_While(expr);
			}
		}
		// Delete
		else if (toks.matchData(toks.begin(), { "delete" }))
		{
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
			{
				if (AST_Name * name = expr->Cast<AST_Name>())
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
			if (AST_Expr * expr = genExpression(toks.between('(', ')').pop_front()))
			{
				if (AST_String * str = expr->Cast<AST_String>())
				{
					return new AST_Include(str);
				}
				delete expr;
			}
		}

		return genExpression(toks);
	}
	
	AST_Expr * Parser::genExpression(const TokenList & toks) const
	{
		if (toks.empty())
		{
			return NULL;
		}
		else if (toks.isWrap('(', ')'))
		{
			return genExpression(toks.unwrapped());
		}
		else if (AST_Float    * temp = generate<AST_Float>(toks))	{ return temp; }
		else if (AST_Int      * temp = generate<AST_Int>(toks))		{ return temp; }
		else if (AST_String   * temp = generate<AST_String>(toks))	{ return temp; }
		else if (AST_Bool     * temp = generate<AST_Bool>(toks))	{ return temp; }
		else if (AST_Name     * temp = generate<AST_Name>(toks))	{ return temp; }
		else if (AST_BinOp	  * temp = genBinOp1(toks))				{ return temp; }
		else if (AST_Member	  * temp = generate<AST_Member>(toks))	{ return temp; }
		else if (AST_New	  * temp = generate<AST_New>(toks))		{ return temp; }
		else if (AST_Struct	  * temp = generate<AST_Struct>(toks))	{ return temp; }
		else if (AST_Command  * temp = generate<AST_Command>(toks))	{ return temp; }
		else if (AST_SizeOf	  * temp = generate<AST_SizeOf>(toks))	{ return temp; }
		else if (AST_TypeID	  * temp = generate<AST_TypeID>(toks))	{ return temp; }
		else if (AST_TypeName * temp = generate<AST_TypeName>(toks)){ return temp; }
		else if (AST_NodeID	  * temp = generate<AST_NodeID>(toks))	{ return temp; }
		else if (AST_Func	  * temp = generate<AST_Func>(toks))	{ return temp; }
		else if (AST_Assign	  * temp = genAssign(toks))				{ return temp; }
		else if (AST_Input	  * temp = generate<AST_Input>(toks))	{ return temp; }
		else if (AST_Call	  * temp = generate<AST_Call>(toks))	{ return temp; }
		else if (AST_Subscr	  * temp = generate<AST_Subscr>(toks))	{ return temp; }
		else if (AST_Array	  * temp = generate<AST_Array>(toks))	{ return temp; }

		TokenList ifx(toks), pfx;
		if (InfixToPostfix(ifx, pfx, m_showItoP))
		{
			if (AST_BinOp * oper = genBinOp2(pfx))
			{
				return oper;
			}
			else if (AST_Expr * expr = genExpression(pfx))
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
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	AST_Assign * Parser::genAssign(const TokenList & toks) const
	{
		TokenList::const_iterator it = toks.begin();

		if (toks.matchStr(it, "n[E]=A"))
		{
			TokenList list({ *(it), *(it + 1), *(it + 2), *(it + 3) });

			if (AST_Subscr* subscr = generate<AST_Subscr>(list))
			{
				if (AST_Expr * value = genExpression(toks.after(5)))
				{
					return new AST_Assign(OperatorType::OP_SET, subscr, value);
				}
			}
		}
		else if (toks.matchStr(it, "n[E]O=A"))
		{
			TokenList list({ *(it), *(it + 1), *(it + 2), *(it + 3) });

			if (AST_Subscr* subscr = generate<AST_Subscr>(list))
			{
				Operator op;
				if (Operator::makeOperator((*(it + 4)).data, (*(it + 5)).data, op))
				{
					if (AST_Expr * value = genExpression(toks.after(6)))
					{
						return new AST_Assign(op, subscr, value);
					}
				}
			}
		}
		else if (toks.matchStr(it, "n.n=A"))
		{
			return new AST_Assign(
				OperatorType::OP_SET,
				new AST_Name(it->data),
				genExpression(toks.after(2))
			);
		}
		else if (toks.matchStr(it, "n.nO=A"))
		{
			Operator op;
			if (Operator::makeOperator((*(it + 1)).data, (*(it + 2)).data, op))
			{
				return new AST_Assign(
					op,
					new AST_Name(it->data),
					genExpression(toks.after(3))
				);
			}
		}
		else if (toks.matchStr(it, "n=A"))
		{
			return new AST_Assign(
				OperatorType::OP_SET,
				new AST_Name(it->data),
				genExpression(toks.after(2))
			);
		}
		else if (toks.matchStr(it, "nO=A"))
		{
			Operator op;
			if (Operator::makeOperator((*(it + 1)).data, (*(it + 2)).data, op))
			{
				return new AST_Assign(
					op,
					new AST_Name(it->data),
					genExpression(toks.after(3))
				);
			}
		}
		return NULL;
	}

	AST_BinOp * Parser::genBinOp1(const TokenList & toks) const
	{
		if (toks.size() == 4 && toks.matchStr(toks.begin(), "EOOE"))
		{
			Operator op;
			if (Operator::makeOperator(toks[1].data, toks[2].data, op))
			{
				return new AST_BinOp(
					op, 
					genExpression(toks.front()),
					genExpression(toks.back()));
			}
		}
		return NULL;
	}

	AST_BinOp *	Parser::genBinOp2(const TokenList & toks) const
	{
		std::stack<AST_Expr *> stk;

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
				stk.push(generate<AST_Call>(call));
			}

			Operator op;
			if (Operator::makeOperator(it->data, op))
			{
				if (stk.size() < 2)
				{
					return NULL;
				}

				AST_Expr * rhs = stk.top();
				stk.pop();
				AST_Expr * lhs = stk.top();
				stk.pop();
				stk.push(new AST_BinOp(op, lhs, rhs));
			}
			else
			{
				stk.push(genExpression(*it));
			}
		}

		if (AST_BinOp* oper = stk.top()->Cast<AST_BinOp>())
		{
			return oper;
		}

		return NULL;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	AST_Array::Values Parser::genArrayElements(const TokenList & toks) const
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
					elems.push_back(genExpression(arg));

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
			elems.push_back(genExpression(arg));
		}

		return elems;
	}

	AST_Call::Params Parser::genCallParams(const TokenList & toks) const
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
					params.push_back(genExpression(arg));
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
			params.push_back(genExpression(arg));
		}

		return params;
	}

	AST_Func::Params Parser::genFuncParams(const TokenList & toks) const
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

	/* * * * * * * * * * * * * * * * * * * * */
}