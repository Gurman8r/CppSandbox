#include <MemeScript/Interpreter.hpp>	
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Command * Interpreter::addCmd(const Command & value)
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

	Command * Interpreter::getCmd(const String & value)
	{
		CommandMap::iterator it;
		if ((it = m_commands.find(value)) != m_commands.begin())
		{
			return &it->second;
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var	Interpreter::execCommand(const String & value)
	{
		if (!value.empty())
		{
			Args args(value, " ");

			CommandMap::iterator it = m_commands.find(args.front());

			if ((it = m_commands.find(args.front())) != m_commands.end())
			{
				return it->second(args);
			}

			return execString(value);
		}
		return Var().voidValue();
	}

	Var	Interpreter::execFile(const String & value)
	{
		if (ML_FileSystem.fileExists(value.c_str()))
		{
			List<char> buffer;
			if (ML_FileSystem.getFileContents(value, buffer))
			{
				return execTokens(lexer().setBuffer(buffer).splitTokens());
			}
		}
		return Var().errorValue("File not found {0}", value);
	}

	Var	Interpreter::execString(const String & value)
	{
		if (!value.empty())
		{
			return execTokens(lexer().setBuffer(value).splitTokens());
		}
		return Var().errorValue("Buffer cannot be empty");
	}

	Var	Interpreter::execTokens(const TokenList & value)
	{
		if (!value.empty())
		{
			if (AST_Block* root = parser().genAST(value))
			{
				return execBlock(root);
			}
		}
		return Var().errorValue("TokenList cannot be empty");
	}

	Var	Interpreter::execBlock(AST_Block * value)
	{
		if (value)
		{
			if (value->run())
			{
				const Var v = (*value->getRet());

				delete value;

				if (runtime().setVar(0, ML_RET, v))
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

	/* * * * * * * * * * * * * * * * * * * * */
}