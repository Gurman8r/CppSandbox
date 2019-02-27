#include <MemeScript/Interpreter.hpp>	
#include <MemeScript/Builtin.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
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
		return Var().voidValue();
	}

	Var	Interpreter::execFile(const String & value)
	{
		if (!value.empty())
		{
			File file;
			if (file.loadFromFile(value))
			{
				return execTokens(lexer.genTokenList(file.data()));
			}
			return Var().errorValue("File not found {0}", value);
		}
		return Var().errorValue("File cannot be empty", value);
	}

	Var	Interpreter::execString(const String & value)
	{
		if (!value.empty())
		{
			return execTokens(lexer.genTokenList(value));
		}
		return Var().errorValue("Buffer cannot be empty");
	}

	Var	Interpreter::execTokens(const TokenList & value)
	{
		if (!value.empty())
		{
			return execTree(lexer.genTokenTree(value));
		}
		return Var().errorValue("TokenList cannot be empty");
	}

	Var Interpreter::execTree(const TokenTree & value)
	{
		if (!value.empty())
		{
			return execBlock(parser.genFromTree(value));
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

				return runtime.setVar(0, ML_NAME_RETV, v)
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

	Var Interpreter::sysCall(TokenList & args)
	{
		// Testing things
		switch (args.front().type)
		{
		case 'n':
		case 's':
		{
			if (args.front("command"))
			{
				return execCommand(args.pop_front().str());
			}
			else if (args.front("config"))
			{
				// group
				const Token group = args.pop();
				switch (group.type)
				{
				case 'n':
				case 's':
				{
					// name
					const Token name = args.pop();
					switch (name.type)
					{
					case 'n':
					case 's':
					{
						// value
						const Token value = args.pop();
						switch (value.type)
						{
						case 'n':
						case 'i':
						case 'f':
						case 's':
						{
							if (group.data == "flag")
							{
								if (name.data == "itop")
								{
									return Var().boolValue(ML_Parser.showItoP(
										StringUtility::ToBool(value.data)));
								}
								else if (name.data == "toks")
								{
									return Var().boolValue(ML_Parser.showToks(
										StringUtility::ToBool(value.data)));
								}
								else if (name.data == "tree")
								{
									return Var().boolValue(ML_Parser.showTree(
										StringUtility::ToBool(value.data)));
								}
								else
								{
									return Var().errorValue("Unknown Name {0}", name.data);
								}
							}
							else
							{
								return Var().errorValue("Unknown Group {0}", group.data);
							}
						}
						break;
						}
					}
					break;
					}
				}
				break;
				}
			}
			else
			{
				return Var().errorValue("Invalid System Call");
			}
		}
		break;
		}
		return Var().voidValue();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}