#include <MemeScript/Script.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Script::Script()
		: m_file	()
		, m_toks	()
		, m_tree	(NULL)
		, m_out		()
	{
	}

	Script::~Script()
	{
		cleanup();
	}


	bool Script::cleanup()
	{
		if (m_tree) 
		{ 
			delete m_tree;
			m_tree = NULL;
		}
		return (!m_tree);
	}

	bool Script::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile((m_path = filename));
	}

	bool Script::build(const Args & args)
	{
		if (m_file)
		{
			m_toks = ML_Interpreter.lexer().setBuffer(m_file.data()).splitTokens();

			if (m_tree = ML_Interpreter.parser().genAST(m_toks))
			{
				if (!args.empty())
				{
					TokenList argToks;
					argToks.push_back(Token('['));
					for (size_t i = 0, imax = args.size(); i < imax; i++)
					{
						argToks.push_back(ML_Interpreter.lexer().makeToken(args[i]));

						if (i < imax - 1)
						{
							argToks.push_back(Token(','));
						}
						else
						{
							argToks.push_back(Token(']'));
						}
					}

					if (AST_Array * argArray = ML_Interpreter.parser().genArray(argToks))
					{
						m_tree->insertChild(0, new AST_Assign(
							OperatorType::OP_SET,
							new AST_Name(ML_ARGS),
							argArray));
					}
				}
				else
				{
					m_tree->insertChild(0, new AST_Assign(
						OperatorType::OP_SET,
						new AST_Name(ML_ARGS),
						new AST_Bool(false)));
				}

				return true;
			}
		}
		m_tree = NULL;
		return false;
	}

	bool Script::run()
	{
		if (m_tree && m_tree->run())
		{
			m_out = m_tree->getRet();

			return cleanup();
		}
		return false;
	}
}