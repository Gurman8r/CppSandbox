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
			m_toks = ML_Lexer.setBuffer(m_file.data()).splitTokens();

			if (m_tree = ML_Parser.genAST(m_toks))
			{
				if (!args.empty())
				{
					TokenList argToks;
					argToks.push_back(Token('['));
					for (size_t i = 0, imax = args.size(); i < imax; i++)
					{
						argToks.push_back(ML_Lexer.makeToken(args[i]));
						argToks.push_back((
							(i < imax - 1)
								? Token(',')
								: Token(']')));
					}

					if (AST_Array * argArray = ML_Parser.generate<AST_Array>(argToks))
					{
						m_tree->insertChild(0, new AST_Assign(
							OperatorType::OP_SET,
							new AST_Name(ML_ARGS),
							argArray));
					}
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