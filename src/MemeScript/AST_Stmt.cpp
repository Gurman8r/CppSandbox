#include <MemeScript/AST_Stmt.h>
#include <MemeScript/AST_Expr.h>
#include <MemeScript/Interpreter.h>

#include <MemeCore/DebugUtility.h>
#include <MemeCore/FileSystem.h>

namespace ml
{
	AST_Stmt::AST_Stmt(AST_Stmt::Type type)
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
			if (AST_Block* b = p->As<AST_Block>())
			{
				return b;
			}
			p = p->getParent();
		}
		return NULL;
	}


	//	Statements
	/* * * * * * * * * * * * * * * * */


	// Elif
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
				std::size_t count = 0;

				while (expr->evaluate().boolValue())
				{
					if (blck->runFirst())
					{
						if (!stmt->run())
						{
							return Debug::LogError("AST_For : Statement Failure");
						}
					}
					else
					{
						return Debug::LogError("AST_For : Block Failure");
					}
				}

				return blck->runNext();
			}
			else
			{
				return Debug::LogError("For: Assignment Failure");
			}
		}
		return Debug::LogError("For Failure");
	}


	// Free
	AST_Free::AST_Free(AST_Name* name)
		: AST_Stmt(ST_Free)
		, name(name)
	{
		addChild(name);
	}

	AST_Free::~AST_Free()
	{
	}

	std::ostream & AST_Free::display(std::ostream & out) const
	{
		return out << "free(" << (*name) << ")";
	}

	bool AST_Free::run()
	{
		if (block()->delv(name->value))
		{
			return runNext();
		}
		return Debug::LogError("AST_Free : Failure {0}", name->value);
	}


	// If
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
					return Debug::LogError("AST_If : Run Body Failed");
				}
			}

			AST_Node* next = iBlock->getNext();

			while (AST_Elif* ei = next->As<AST_Elif>())
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
							return Debug::LogError("AST_Elif : Run Body Failed");
						}
					}
					else
					{
						next = eiBlock->getNext();
					}
				}
				else
				{
					return Debug::LogError("AST_Elif : Body Not Found");
				}
			}

			if (AST_Else* el = next->As<AST_Else>())
			{
				if (AST_Block* elBlock = el->nextAs<AST_Block>())
				{
					if (elBlock->runFirst())
					{
						return elBlock->runNext();
					}
					else
					{
						return Debug::LogError("AST_Else : Run Body Failed");
					}
				}
				else
				{
					return Debug::LogError("AST_Else : Body Not Found");
				}
			}

			return runNext();
		}
		else
		{
			return Debug::LogError("AST_If : Body Not Found");
		}
	}


	// Include
	AST_Include::AST_Include(AST_String* str)
		: AST_Stmt(AST_Stmt::Type::ST_Include)
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
		const std::string& filename = str->evaluate().stringValue();
		
		std::vector<char> buffer;
		if (ML_FileSystem.getFileContents(filename, buffer))
		{
			const TokenList toks = ML_Interpreter.lexer()->setBuffer(buffer).splitTokens();
			if (AST_Block * root = ML_Interpreter.parser()->genAST(toks))
			{
				if (!root->empty())
				{
					AST_Node * prev = this;

					AST_Node::const_iterator it;
					for (it = root->begin(); it != root->end(); it++)
					{
						if (!block()->insertChildAfter(prev, (*it)))
						{
							Debug::LogError("AST_Include : Failed loading node");
						}

						prev = (*it);
					}
				}
				else
				{
					Debug::LogWarning("AST_Include : Nothing to load");
				}

				root->clear();
				delete root;
			}
			return runNext();
		}
		return Debug::LogError("AST_Include : File Not Found");
	}


	// Print
	AST_Print::AST_Print(AST_Expr * expr)
		: AST_Stmt(ST_Print)
		, expr(expr)
	{
		addChild(expr);
	}

	AST_Print::~AST_Print()
	{
	}

	std::ostream & AST_Print::display(std::ostream & out) const
	{
		return out << "print(" << *expr << ") ";// << (getParent() ? getParent()->getID() : 0);
	}

	bool AST_Print::run()
	{
		if (AST_String* str = expr->As<AST_String>())
		{
			std::string::const_iterator it;
			for (it = str->value.begin(); it != str->value.end(); it++)
			{
				if (*it == '{')
				{
					std::string name;
					while (++it != str->value.end())
					{
						if (*it == '}')
						{
							break;
						}

						name += (*it);
					}

					if (Var * v = block()->getv(name))
					{
						Debug::out() << (*v).textValue();
					}
					else
					{
						Debug::out() << name;
					}
				}
				else
				{
					Debug::out() << (*it);
				}
			}
		}
		else
		{
			Debug::out() << expr->evaluate().textValue();
		}
		
		Debug::out() << FMT() << std::endl;
		
		return runNext();
	}


	// Return
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
			return true;
		}
		return Debug::LogError("AST_Return : Failed Setting Value");
	}


	// While
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
			std::size_t count = 0;
			while (expr->evaluate().boolValue())
			{
				if (blck->runFirst())
				{
					if (++count >= 100)
					{
						return Debug::LogError("AST_While : Loops Exceded {0}", count);
					}
				}
				else
				{
					return Debug::LogError("AST_While : Block Run First Failed");
				}
			}
			return blck->runNext();
		}
		return Debug::LogError("AST_While : Failure");
	}
	
}