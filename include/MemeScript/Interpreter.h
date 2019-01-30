#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <MemeCore/ISingleton.h>
#include <MemeCore/StringUtility.h>
#include <MemeScript/Lexer.h>
#include <MemeScript/Parser.h>
#include <MemeScript/Runtime.h>
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
		using CmdTable = std::unordered_map<String, Command>;
		using CmdNames = std::vector<String>;

	private:
		Interpreter() {}
		~Interpreter() {}

	public:
		template <typename T, typename ... A>
		inline Var execCommand(const String& fmt, const T& arg0, const A&... args)
		{
			return execCommand(StringUtility::Format(fmt, arg0, (args)...));
		}

		Var	execCommand(const String & value);
		Var	execFile(const String & value);
		Var	execSource(const String & value);
		Var	execToks(const TokenList & value);
		Var	execAST(AST_Block * value);

		Command * addCmd(const Command & value);
		Command * getCmd(const String & value);

		inline const CmdTable & cmdTable() const { return m_cmdTable; }
		inline const CmdNames & cmdNames() const { return m_cmdNames; }

		inline Lexer   & lexer()	{ return m_lexer; }
		inline Runtime & runtime()	{ return m_runtime; }
		inline Parser  & parser()	{ return m_parser; }

	private:
		CmdTable	m_cmdTable;
		CmdNames	m_cmdNames;
		Lexer		m_lexer;
		Runtime		m_runtime;
		Parser		m_parser;
	};
}

#endif // !_INTERPRETER_H_
