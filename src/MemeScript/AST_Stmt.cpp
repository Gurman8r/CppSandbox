#include <MemeScript/AST_Stmt.hpp>
#include <MemeScript/AST_Expr.hpp>
#include <MemeScript/Interpreter.hpp>

#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	// Statement
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Stmt::AST_Stmt(int32_t type)
		: AST_Node()
		, stmtType(type)
	{
	}

	AST_Stmt::~AST_Stmt()
	{
	}

	std::ostream & AST_Stmt::display(std::ostream & out) const
	{
		return AST_Node::display(out);
	}

	AST_Block * AST_Stmt::block() const
	{
		AST_Node* p = getParent();
		while (p)
		{
			if (AST_Block* b = p->Cast<AST_Block>())
			{
				return b;
			}
			p = p->getParent();
		}
		return NULL;
	}


	// Elif
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Elif::AST_Elif(AST_Expr * expr)
		: AST_Stmt(ST_Elif)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_Elif::~AST_Elif()
	{
	}

	std::ostream & AST_Elif::display(std::ostream & out) const
	{
		return out << "elif(" << *expr << ")";
	}

	bool AST_Elif::run()
	{
		return runNext();
	}


	// Else
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Else::AST_Else()
		: AST_Stmt(ST_Else)
	{
	}

	AST_Else::~AST_Else()
	{
	}

	std::ostream & AST_Else::display(std::ostream & out) const
	{
		return out << "else";
	}

	bool AST_Else::run()
	{
		return runNext();
	}


	// For
	/* * * * * * * * * * * * * * * * * * * * */
	AST_For::AST_For(AST_Assign * assn, AST_Expr * expr, AST_Stmt * stmt)
		: AST_Stmt(ST_For)
		, assn(assn)
		, expr(expr)
		, stmt(stmt)
	{
		addChild(assn);
		addChild(expr);
		addChild(stmt);
	}

	AST_For::~AST_For()
	{
	}

	std::ostream & AST_For::display(std::ostream & out) const
	{
		return out << "for(" << (*assn) << "; " << (*expr) << "; " << (*stmt) << ")";
	}

	bool AST_For::run()
	{
		if (AST_Block* blck = nextAs<AST_Block>())
		{
			if (assn->run())
			{
				size_t count = 0;

				while (expr->evaluate().boolValue())
				{
					if (blck->runFirst())
					{
						if (!stmt->run())
						{
							return Debug::logError("AST_For : Statement Failure");
						}
					}
					else
					{
						return Debug::logError("AST_For : Block Failure");
					}
				}

				return blck->runNext();
			}
			else
			{
				return Debug::logError("For: Assignment Failure");
			}
		}
		return Debug::logError("For Failure");
	}


	// Free
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Delete::AST_Delete(AST_Name* name)
		: AST_Stmt(ST_Delete)
		, name(name)
	{
		addChild(name);
	}

	AST_Delete::~AST_Delete()
	{
	}

	std::ostream & AST_Delete::display(std::ostream & out) const
	{
		return out << "delete(" << (*name) << ")";
	}

	bool AST_Delete::run()
	{
		if (block()->delVar(name->value))
		{
			return runNext();
		}
		return Debug::logError("AST_Delete : Failure {0}", name->value);
	}


	// If
	/* * * * * * * * * * * * * * * * * * * * */
	AST_If::AST_If(AST_Expr * expr)
		: AST_Stmt(ST_If)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_If::~AST_If()
	{
	}

	std::ostream & AST_If::display(std::ostream & out) const
	{
		return out << "if(" << *expr << ")";
	}

	bool AST_If::run()
	{
		if (AST_Block* iBlock = nextAs<AST_Block>())
		{
			if (expr->evaluate().boolValue())
			{
				if (iBlock->runFirst())
				{
					return iBlock->runNext();
				}
				else
				{
					return Debug::logError("AST_If : Run Body Failed");
				}
			}

			AST_Node* next = iBlock->getNext();

			while (AST_Elif* ei = next->Cast<AST_Elif>())
			{
				if (AST_Block* eiBlock = ei->nextAs<AST_Block>())
				{
					if (ei->expr->evaluate().boolValue())
					{
						if (eiBlock->runFirst())
						{
							return eiBlock->runNext();
						}
						else
						{
							return Debug::logError("AST_Elif : Run Body Failed");
						}
					}
					else
					{
						next = eiBlock->getNext();
					}
				}
				else
				{
					return Debug::logError("AST_Elif : Body Not Found");
				}
			}

			if (AST_Else* el = next->Cast<AST_Else>())
			{
				if (AST_Block* elBlock = el->nextAs<AST_Block>())
				{
					if (elBlock->runFirst())
					{
						return elBlock->runNext();
					}
					else
					{
						return Debug::logError("AST_Else : Run Body Failed");
					}
				}
				else
				{
					return Debug::logError("AST_Else : Body Not Found");
				}
			}

			return runNext();
		}
		else
		{
			return Debug::logError("AST_If : Body Not Found");
		}
	}


	// Include
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Include::AST_Include(AST_String* str)
		: AST_Stmt(ST_Include)
		, str(str)
	{
		addChild(str);
	}

	AST_Include::~AST_Include()
	{
	}

	std::ostream & AST_Include::display(std::ostream & out) const
	{
		return out << "include(" << (*str) << ")";
	}

	bool AST_Include::run()
	{
		const String& filename = str->evaluate().stringValue();
		
		List<char> buffer;
		if (ML_FileSystem.getFileContents(filename, buffer))
		{
			const TokenList toks = ML_Interpreter.lexer().setBuffer(buffer).splitTokens();
			if (AST_Block * root = ML_Interpreter.parser().genAST(toks))
			{
				if (!root->empty())
				{
					AST_Node * prev = this;

					AST_Node::const_iterator it;
					for (it = root->begin(); it != root->end(); it++)
					{
						if (!block()->insertChildAfter(prev, (*it)))
						{
							Debug::logError("AST_Include : Failed loading node");
						}

						prev = (*it);
					}
				}
				else
				{
					Debug::logWarning("AST_Include : Nothing to load");
				}

				root->clearChildren();
				delete root;
			}
			return runNext();
		}
		return Debug::logError("AST_Include : File Not Found");
	}


	// Print
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Print::AST_Print(AST_Expr * expr, bool newl)
		: AST_Stmt(ST_Print)
		, expr(expr)
		, newl(newl)
	{
		addChild(expr);
	}

	AST_Print::~AST_Print()
	{
	}

	std::ostream & AST_Print::display(std::ostream & out) const
	{
		return out << (newl ? "printl" : "print") << "(" << *expr << ") ";
	}

	bool AST_Print::run()
	{
		if (AST_String* str = expr->Cast<AST_String>())
		{
			String::const_iterator it;
			for (it = str->value.begin(); it != str->value.end(); it++)
			{
				if (*it == '{')
				{
					String name;
					while (++it != str->value.end())
					{
						if (*it == '}')
						{
							break;
						}
						name += (*it);
					}

					if (Var * v = block()->getVar(name))
					{
						cout << (*v).textValue();
					}
					else
					{
						cout << name;
					}
				}
				else
				{
					cout << (*it);
				}
			}
		}
		else
		{
			cout << expr->evaluate().textValue();
		}
		
		cout << FMT();
		
		if(newl) { cout << ml::endl; }
		
		return runNext();
	}


	// Return
	/* * * * * * * * * * * * * * * * * * * * */
	AST_Return::AST_Return(AST_Expr * expr)
		: AST_Stmt(ST_Return)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_Return::~AST_Return()
	{
	}

	std::ostream & AST_Return::display(std::ostream & out) const
	{
		return out << "return " << (*expr);
	}

	bool AST_Return::run()
	{
		if (block()->setRet(expr->evaluate()))
		{
			return true; // FIXME: Only halts containing block
		}
		return Debug::logError("AST_Return : Failed Setting value_type");
	}


	// While
	/* * * * * * * * * * * * * * * * * * * * */
	AST_While::AST_While(AST_Expr * expr)
		: AST_Stmt(ST_While)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_While::~AST_While()
	{
	}

	std::ostream & AST_While::display(std::ostream & out) const
	{
		return out << "while(" << (*expr) << ")";
	}

	bool AST_While::run()
	{
		if (AST_Block* blck = nextAs<AST_Block>())
		{
			size_t count = 0;
			while (expr->evaluate().boolValue())
			{
				if (blck->runFirst())
				{
					if (++count >= 100)
					{
						return Debug::logError("AST_While : Loops Exceded {0}", count);
					}
				}
				else
				{
					return Debug::logError("AST_While : Block Run First Failed");
				}
			}
			return blck->runNext();
		}
		return Debug::logError("AST_While : Failure");
	}
	
}