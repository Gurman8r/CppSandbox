#ifndef _INTERPRETER_HPP_
#define _INTERPRETER_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeScript/StringUtility.hpp>
#include <MemeScript/Lexer.hpp>
#include <MemeScript/Parser.hpp>
#include <MemeScript/Runtime.hpp>
#include <MemeScript/Command.hpp>

#define ML_Interpreter	ml::Interpreter::getInstance()
#define ML_Lexer		ML_Interpreter.lexer
#define ML_Parser		ML_Interpreter.parser
#define ML_Runtime		ML_Interpreter.runtime

namespace ml
{
	class ML_SCRIPT_API Interpreter final
		: public ITrackable
		, public ISingleton<Interpreter>
	{	
		friend ISingleton<Interpreter>;

	public:
		using CommandMap  = HashMap<String, Command>;
		using CommandPair = Pair<String, Command>;

	public:
		Command * install(const Command & value);
		Command * getCommand(const String & value);

		template <typename T, typename ... A>
		inline Var execCommand(const String & fmt, const T& arg0, const A&... args)
		{
			return execCommand(fmt.format(arg0, (args)...));
		}

		Var	execCommand(const String & value);
		Var	execFile(const String & value);
		Var	execString(const String & value);
		Var	execTokens(const TokenList & value);
		Var execTree(const TokenTree & value);
		Var	execBlock(AST_Block * value);

		Var system_callback(TokenList & args);

		inline const CommandMap & commands() const { return m_commands; }

	public:
		Lexer   lexer;
		Runtime runtime;
		Parser  parser;

	private:
		Interpreter() {}
		~Interpreter() {}

		CommandMap m_commands;
	};
}

#endif // !_INTERPRETER_HPP_
