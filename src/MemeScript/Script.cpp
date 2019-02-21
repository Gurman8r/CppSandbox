#include <MemeScript/Script.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	Script::Script()
		: m_file	()
		, m_tokens	()
		, m_ast		(NULL)
		, m_ret		()
	{
	}

	Script::~Script()
	{
		cleanup();
	}


	bool Script::cleanup()
	{
		if (m_ast) { delete m_ast; }

		return m_file.cleanup();
	}

	bool Script::loadFromFile(const String & filename)
	{
		static Lexer lexer;
		static Parser parser;

		if (m_file.loadFromFile(filename))
		{
			m_tokens = lexer.setBuffer(m_file.data()).splitTokens();

			if (m_ast = parser.genAST(m_tokens))
			{
				return true;
			}
		}
		return false;
	}

	bool Script::run()
	{
		if (m_ast && m_ast->run())
		{
			m_ret = m_ast->getRet();
			
			return !(m_ret.isErrorType());
		}
		return false;
	}
}