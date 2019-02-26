#include <MemeScript/Script.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{

	/* * * * * * * * * * * * * * * * * * * * */
	Script::Script()
		: m_file	()
		, m_toks	()
		, m_root	(NULL)
		, m_out		()
	{
	}

	Script::~Script()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Script::cleanup()
	{
		if (m_root) 
		{ 
			delete m_root;
			m_root = NULL;
		}
		return (!m_root);
	}

	bool Script::loadFromFile(const String & filename)
	{
		return m_file.loadFromFile((m_path = filename));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Script::build(const Args & args)
	{
		if (m_file)
		{
			// Generate Tokens
			m_toks = ML_Lexer.genTokenList(m_file.data());

			// Generate Tree
			if (m_root = ML_Parser.genFromList(m_toks))
			{
				// Generate __ARGS__ array
				if (m_root->insertChild(0, new AST_Assign(
					Operator::OP_SET,
					ML_Parser.generate<AST_Name>(Token('n', ML_ARGS)),
					ML_Parser.generate<AST_Array>(ML_Lexer.genArgsArray(args)))))
				{
					return true;
				}
			}
		}
		m_root = NULL;
		return false;
	}

	bool Script::rebuild(const Args & args)
	{
		return loadFromFile(m_path) && build(args);
	}

	bool Script::run()
	{
		if (m_root && m_root->run())
		{
			m_out = m_root->getRet();

			return cleanup();
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}