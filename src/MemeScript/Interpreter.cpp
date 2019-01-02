#include <MemeScript/Interpreter.h>	
#include <MemeCore/FileSystem.h>

namespace ml
{
	Interpreter::Interpreter()
	{
		m_memory = new Memory();
		m_lexer = new Lexer();
		m_parser = new Parser();
	}

	Interpreter::~Interpreter()
	{
		delete m_memory;
		delete m_parser;
		delete m_lexer;
	}


	Var	Interpreter::execCommand(const std::string& value)
	{
		if (!value.empty())
		{
			const Args args(value, " ");

			CommandMap::iterator it = m_commands.find(args.front());

			if (it != m_commands.end())
			{
				return it->second(args);
			}

			return execSyntax(value);
		}
		return Var().voidValue();
	}

	Var	Interpreter::execScript(const std::string & value)
	{
		if (FileSystem::fileExists(value.c_str()))
		{
			std::vector<char> buffer;
			if (FileSystem::getFileContents(value, buffer))
			{
				return execTokens(lexer()->setBuffer(buffer).splitTokens());
			}
		}
		return Var().errorValue("File not found {0}", value);
	}

	Var	Interpreter::execSyntax(const std::string & value)
	{
		if (!value.empty())
		{
			return execTokens(lexer()->setBuffer(value).splitTokens());
		}
		return Var().errorValue("Buffer cannot be empty");
	}

	Var	Interpreter::execTokens(const TokenList & value)
	{
		if (!value.empty())
		{
			if (AST_Block* root = parser()->genAST(value))
			{
				return execAST(root);
			}
		}
		return Var().errorValue("TokenList cannot be empty");
	}

	Var	Interpreter::execAST(AST_Block * value)
	{
		if (value)
		{
			if (value->run())
			{
				const Var v = (*value->getRet());

				delete value;

				if (memory()->setVar(0, "?", v))
				{
					return v;
				}
				else
				{
					return Var().errorValue("Failed setting return");
				}
			}
			else
			{
				return Var().errorValue("Failed running value");
			}
		}
		else
		{
			return Var().errorValue("Root cannot be null");
		}
	}


	Command*Interpreter::addCommand(const Command & value)
	{
		if (!value.name().empty())
		{
			if (m_commands.find(value.name()) == m_commands.end())
			{
				m_commands.insert({ value.name(), value });

				return &m_commands[value.name()];
			}
		}

		return NULL;
	}

	Command*Interpreter::getCommand(const std::string & value)
	{
		CommandMap::iterator it = m_commands.find(value);
		if (it != m_commands.begin())
		{
			return &it->second;
		}

		return NULL;
	}
	
	
	const std::vector<std::string> & Interpreter::getCmdNames() const
	{
		static std::vector<std::string> names;

		uint32_t size = m_commands.size();

		if (names.size() != size)
		{
			names.resize(size);

			uint32_t i = 0;
			for (const auto & pair : m_commands)
			{
				names[i++] = pair.first;
			}
		}

		return names;
	}
}