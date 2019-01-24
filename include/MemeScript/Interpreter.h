#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/StringUtility.h>
#include <MemeScript/Lexer.h>
#include <MemeScript/Parser.h>
#include <MemeScript/Memory.h>
#include <MemeScript/Command.h>

#define ML_Interpreter ml::Interpreter::getInstance()

namespace ml
{
	class ML_SCRIPT_API Interpreter final
		: public ITrackable
		, public ISingleton<Interpreter>
	{	
		friend ISingleton<Interpreter>;

	public:
		using CmdTable = std::unordered_map<std::string, Command>;
		using CmdNames = std::vector<std::string>;

	private:
		Interpreter();
		~Interpreter();

	public:
		template <typename T, typename ... A>
		inline Var execCommand(const std::string& fmt, const T& arg0, const A&... args)
		{
			return execCommand(StringUtility::Format(fmt, arg0, (args)...));
		}

		Var	execCommand(const std::string & value);
		Var	execScript(const std::string & value);
		Var	execSyntax(const std::string & value);
		Var	execTokens(const TokenList & value);
		Var	execAST(AST_Block * value);

		Command * addCmd(const Command & value);
		Command * getCmd(const std::string & value);

		const CmdNames & cmdNames() const { return m_cmdNames; }

		const Lexer*	lexer()  const	{ return m_lexer;  }
		const Memory*	memory() const	{ return m_memory; }
		const Parser*	parser() const	{ return m_parser; }

		Lexer*			lexer()			{ return m_lexer;  }
		Memory*			memory()		{ return m_memory; }
		Parser*			parser()		{ return m_parser; }

	private:
		CmdTable m_cmdTable;
		CmdNames m_cmdNames;

		Memory*		m_memory;
		Lexer*		m_lexer;
		Parser*		m_parser;
	};
}

#endif // !_INTERPRETER_H_
