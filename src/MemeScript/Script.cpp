#include <MemeScript/Script.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeScript/ScriptMacros.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: m_file	()
		, m_toks	()
		, m_root	(NULL)
		, m_retv	()
	{
	}

	Script::~Script()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Script::dispose()
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

	bool Script::loadFromMemory(const File & file)
	{
		if (file)
		{
			return true;
		}
		return false;
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
				// __ARGS__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_NAME_ARGS),
					ML_Parser.generate<AST_Array>(ML_Lexer.genArgsArray(args)))
				);

				// __FILE__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_NAME_FILE),
					new AST_String(m_path))
				);

				// __PATH__
				m_root->push_front(new AST_Assign(
					Operator::OP_SET,
					new AST_Name(ML_NAME_PATH),
					new AST_String(ML_FS.getPathTo("")))
				);

				return true;
			}
		}
		m_root = NULL;
		return false;
	}

	bool Script::buildAndRun(const Args & args)
	{
		return build(args) && run();
	}

	bool Script::rebuild(const Args & args)
	{
		return loadFromFile(m_path) && build(args);
	}

	bool Script::run()
	{
		if (m_root)
		{
			if (m_root->run())
			{
				m_retv = m_root->getRet();

				return dispose();
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}