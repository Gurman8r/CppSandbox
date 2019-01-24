#include <MemeScript/AST_Block.h>
#include <MemeScript/AST_Expr.h>
#include <MemeScript/Interpreter.h>
#include <MemeCore/DebugUtility.h>


namespace ml
{
	AST_Block::AST_Block()
		: AST_Node()
	{
		ML_Interpreter.runtime()->makeScope(getID());
	}

	AST_Block::AST_Block(const list_type & body)
		: AST_Node(body)
	{
		ML_Interpreter.runtime()->makeScope(getID());
	}

	AST_Block::~AST_Block()
	{
		ML_Interpreter.runtime()->clearScope(getID());
	}


	std::ostream & AST_Block::display(std::ostream & out) const
	{
		auto tab = [](std::size_t n)
		{
			return (n == 0)
				? std::string()
				: std::string(n, '\t');
		};

		out << "{\n";

		for (AST_Node* n : (*this))
		{
			out << tab(n->getDepth()) << (*n) 
				<< std::endl << FMT();
		}

		out << tab(getDepth()) << "}";

		return out;
	}


	bool	AST_Block::delv(const std::string & name)
	{
		if (ML_Interpreter.runtime()->delVar(getID(), name))
		{
			return true;
		}
		else if (AST_Block* b = block())
		{
			return b->delv(name);
		}
		else
		{
			return false;
		}
	}

	Var *	AST_Block::getv(const std::string & name) const
	{
		if (Var * v = ML_Interpreter.runtime()->getVar(getID(), name))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->getv(name);
		}
		else
		{
			return NULL;
		}
	}

	Var *	AST_Block::newv(const std::string & name, const Var & value)
	{
		if (Var * v = ML_Interpreter.runtime()->newVar(getID(), name, value))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->newv(name, value);
		}
		else
		{
			return NULL;
		}
	}

	Var *	AST_Block::setv(const std::string & name, const Var & value)
	{
		if (Var * v = getv(name))
		{
			(*v) = value;
			return v;
		}
		else if (Var * v = ML_Interpreter.runtime()->setVar(getID(), name, value))
		{
			return v;
		}
		else if (AST_Block* b = block())
		{
			return b->setv(name, value);
		}
		else
		{
			return NULL;
		}
	}


	Var *	AST_Block::getRet() const
	{
		if (Var * v = ML_Interpreter.runtime()->getVar(getID(), "?"))
		{
			return v;
		}
		return setRet(Var().voidValue());
	}

	Var *	AST_Block::setRet(const Var & value) const
	{
		return ML_Interpreter.runtime()->setVar(getID(), "?", value);
	}


	bool		AST_Block::addFunc(const std::string & name, AST_Func * func)
	{
		if (!getFunc(name))
		{
			if (Var * v = setv(name, Var().funcValue({ { TokenType::TOK_NAME, name } })))
			{
				m_funcs.insert({ name, func });

				return true;
			}
		}
		return false;
	}

	AST_Func*	AST_Block::getFunc(const std::string & name)
	{
		if (AST_Block* b = block())
		{
			if (AST_Func* f = b->getFunc(name))
			{
				return f;
			}
		}

		FuncMap::iterator it = m_funcs.find(name);
		if (it != m_funcs.end())
		{
			return it->second;
		}

		return NULL;
	}

	AST_Block::FuncMap AST_Block::getFuncs() const
	{
		return m_funcs;
	}


	AST_Block*	AST_Block::block() const
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

	Var			AST_Block::evaluate() const
	{
		return (*getRet());
	}

	bool		AST_Block::run()
	{
		// dont run block
		if (prevAs<AST_If>() ||
			prevAs<AST_Elif>() ||
			prevAs<AST_Else>() ||
			prevAs<AST_For>() ||
			prevAs<AST_While>() ||
			prevAs<AST_Func>())
		{
			return runNext();
		}

		return runFirst();
	}

	bool		AST_Block::runFirst()
	{
		if (AST_Node* n = getChild(0))
		{
			return n->run();
		}
		return Debug::LogError("Block is empty");
	}

}