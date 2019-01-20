#include <MemeScript/Interpreter.h>	
#include <MemeCore/FileSystem.h>
#include <MemeCore/ConsoleUtility.h>
#include <dirent.h>
#include <MemeCore/DebugUtility.h>

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
			Args args(value, " ");

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
		if (ML_FileSystem.fileExists(value.c_str()))
		{
			std::vector<char> buffer;
			if (ML_FileSystem.getFileContents(value, buffer))
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

		std::size_t size = m_commands.size();

		if (names.size() != size)
		{
			names.resize(size);

			std::size_t i = 0;
			for (const auto & pair : m_commands)
			{
				names[i++] = pair.first;
			}
		}

		return names;
	}


	void Interpreter::LoadBuiltinCommands()
	{
		ML_Interpreter.addCommand(ml::Command("help", [](ml::Args & args)
		{
			for (auto n : ML_Interpreter.getCmdNames())
				Debug::out() << n << std::endl;
			return ml::Var().boolValue(true);
		}));

		ML_Interpreter.addCommand(ml::Command("pause", [](ml::Args & args)
		{
			return ml::Var().intValue(ml::Debug::pause());
		}));

		ML_Interpreter.addCommand(ml::Command("clear", [](ml::Args & args)
		{
			return ml::Var().intValue(Debug::clear());
		}));

		ML_Interpreter.addCommand(ml::Command("cd", [](ml::Args & args)
		{
			return ml::Var().boolValue(ML_FileSystem.changeDir(args.pop_front().front()));
		}));

		ML_Interpreter.addCommand(ml::Command("exist", [](ml::Args & args)
		{
			return ml::Var().boolValue(ML_FileSystem.fileExists(args.pop_front().front()));
		}));

		ML_Interpreter.addCommand(ml::Command("dir", [](ml::Args & args)
		{
			std::string dName = args.pop_front().empty() ? "./" : args.str();

			if (DIR* dir = opendir(dName.c_str()))
			{
				dirent* e;
				while ((e = readdir(dir)))
				{
					switch (e->d_type)
					{
					case DT_REG:
						Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "";
						break;
					case DT_DIR:
						Debug::out() << (ml::FG::Blue | ml::BG::Green) << e->d_name << "/";
						break;
					case DT_LNK:
						Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "@";
						break;
					default:
						Debug::out() << (ml::FG::Green | ml::BG::Black) << e->d_name << "*";
						break;
					}
					Debug::out() << ml::FMT() << std::endl;
				}
				closedir(dir);
				return ml::Var().boolValue(true);
			}
			Debug::out() << "Dir \'" << dName << "\' does not exist." << std::endl;
			return ml::Var().boolValue(false);
		}));
	}
}