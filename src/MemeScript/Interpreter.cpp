#include <MemeScript/Interpreter.h>	
#include <MemeCore/FileSystem.h>

#include <dirent.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	Var	Interpreter::execCommand(const std::string & value)
	{
		if (!value.empty())
		{
			Args args(value, " ");

			CmdTable::iterator it = m_cmdTable.find(args.front());

			if (it != m_cmdTable.end())
			{
				return it->second(args);
			}

			return execSource(value);
		}
		return Var().voidValue();
	}

	Var	Interpreter::execFile(const std::string & value)
	{
		if (ML_FileSystem.fileExists(value.c_str()))
		{
			std::vector<char> buffer;
			if (ML_FileSystem.getFileContents(value, buffer))
			{
				return execToks(lexer()->setBuffer(buffer).splitTokens());
			}
		}
		return Var().errorValue("File not found {0}", value);
	}

	Var	Interpreter::execSource(const std::string & value)
	{
		if (!value.empty())
		{
			return execToks(lexer()->setBuffer(value).splitTokens());
		}
		return Var().errorValue("Buffer cannot be empty");
	}

	Var	Interpreter::execToks(const TokenList & value)
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

				if (runtime()->setVar(0, "?", v))
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


	Command*Interpreter::addCmd(const Command & value)
	{
		if (!value.name().empty())
		{
			if (m_cmdTable.find(value.name()) == m_cmdTable.end())
			{
				m_cmdTable.insert({ value.name(), value });

				m_cmdNames.push_back(value.name());

				return &m_cmdTable[value.name()];
			}
		}
		return NULL;
	}

	Command*Interpreter::getCmd(const std::string & value)
	{
		CmdTable::iterator it;
		if ((it = m_cmdTable.find(value)) != m_cmdTable.begin())
		{
			return &it->second;
		}
		return NULL;
	}
}