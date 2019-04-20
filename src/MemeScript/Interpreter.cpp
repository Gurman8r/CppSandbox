#include <MemeScript/Interpreter.hpp>	
#include <MemeScript/ScriptMacros.hpp>
#include <MemeScript/ScriptEvents.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/EventSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Interpreter::Interpreter()
	{
		ML_EventSystem.addListener(ScriptEvent::EV_Command, this);
	}

	Interpreter::~Interpreter()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Interpreter::onEvent(const IEvent * value)
	{
		switch (*value)
		{
		case ScriptEvent::EV_Command:
			if (const auto * ev = value->as<CommandEvent>())
			{
				if (execCommand(ev->cmd).isErrorType())
				{
					Debug::logError(ev->cmd);
				}
			}
			break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Command * Interpreter::install(const Command & value)
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

	Command * Interpreter::getCommand(const String & value)
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

			if (value.find(';') != String::npos)
			{
				return execString(value);
			}
		}
		return Var().errorValue("Unknown Command ", value);
	}

	Var	Interpreter::execFile(const String & value)
	{
		if (!value.empty())
		{
			File file;
			if (file.loadFromFile(value))
			{
				return execTokens(ML_Lexer.genTokenList(file.data()));
			}
			return Var().errorValue("File not found {0}", value);
		}
		return Var().errorValue("File cannot be empty", value);
	}

	Var	Interpreter::execString(const String & value)
	{
		if (!value.empty())
		{
			return execTokens(ML_Lexer.genTokenList(value));
		}
		return Var().errorValue("Buffer cannot be empty");
	}

	Var	Interpreter::execTokens(const TokenList & value)
	{
		if (!value.empty())
		{
			return execTree(ML_Lexer.genTokenTree(value));
		}
		return Var().errorValue("TokenList cannot be empty");
	}

	Var Interpreter::execTree(const TokenTree & value)
	{
		if (!value.empty())
		{
			return execBlock(ML_Parser.genFromTree(value));
		}
		return Var().errorValue("Statements cannot be empty");
	}

	Var	Interpreter::execBlock(AST_Block * value)
	{
		if (value)
		{
			if (value->run())
			{
				const Var v = (*value->getRet());

				delete value;

				return ML_Runtime.setVar(0, ML_NAME_RETV, v)
					? v
					: Var().errorValue("Interpreter : Failed setting return");
			}
			else
			{
				return Var().errorValue("Interpreter : Failed running Block");
			}
		}
		else
		{
			return Var().errorValue("Interpreter : Root cannot be null");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}