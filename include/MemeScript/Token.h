#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <MemeScript/Args.h>
#include <unordered_map>

namespace ml
{
	struct ML_SCRIPT_API Token final
		: public ITrackable
	{
		enum Type : char
		{
			TOK_ERR = '\0', // \0 - Invalid / Null
			TOK_VOID = ' ',	// ' ' - Empty / Space
			TOK_ENDL = '\n',	// \n - End of line

			TOK_NAME = 'n',	// n - std::string
			TOK_FLT = 'f',	// f - Float / Decimal
			TOK_INT = 'i',	// i - Integer
			TOK_STR = 's',	// s - String

			TOK_POW = '^',	// ^ - Pow
			TOK_MUL = '*',	// * - Multiply
			TOK_DIV = '/',	// / - Divide
			TOK_ADD = '+',	// + - Add
			TOK_SUB = '-',	// - - Subtract
			TOK_EQU = '=',	// = - Equal
			TOK_LT = '<',	// < - Less Than
			TOK_GT = '>',	// > - Greater Than
			TOK_NOT = '!',	// ! - Not

			TOK_OR = '|',	// | - Or
			TOK_AND = '&',	// & - And
			TOK_MOD = '%',	// % - Mod

			TOK_HASH = '#',	// # - Hash
			TOK_TERM = '\\',	// \ - erminate
			TOK_SEMI = ';',	// ; - Semicolon
			TOK_CMMA = ',',	// , - Comma
			TOK_COLN = ':',	// : - Colon

			TOK_LPRN = '(',	// ( - L Paren
			TOK_RPRN = ')',	// ) - R Paren
			TOK_LBRC = '{',	// { - L Brace
			TOK_RBRC = '}',	// } - R Brace
			TOK_LBKT = '[',	// [ - L Bracket
			TOK_RBKT = ']',	// ] - R Bracket

			TOK_DQTE = '\"', // " - Double Quote
			TOK_SQTE = '\'', // ' - Single Quote

			MAX_TOKEN_TYPE
		};

		using SymbolMap = std::unordered_map<std::string, Type>;
		using NameMap = std::unordered_map<Token::Type, std::string>;

		static const NameMap	TypeNames;
		static const SymbolMap	Symbols;

		Type		type;
		std::string data;

		Token();
		Token(Type type);
		Token(char type);
		Token(Type type, const std::string& data);
		Token(char type, const std::string& data);
		Token(const Token& copy);

		Token& operator=(const Token& value);
		Token& operator=(const Token::Type& value);

		bool isOperand() const;
		bool isOperator() const;
		std::string str() const;

		friend std::ostream& operator<<(std::ostream& out, const Token::Type& rhs);
		friend std::ostream& operator<<(std::ostream& out, const Token& tok);

		friend bool operator==(const Token& lhs, const Token& rhs);
		friend bool operator!=(const Token& lhs, const Token& rhs);
		friend bool operator >(const Token& lhs, const Token& rhs);
		friend bool operator <(const Token& lhs, const Token& rhs);
		friend bool operator>=(const Token& lhs, const Token& rhs);
		friend bool operator<=(const Token& lhs, const Token& rhs);

		friend bool operator==(const Token& lhs, const Token::Type& rhs);
		friend bool operator!=(const Token& lhs, const Token::Type& rhs);
		friend bool operator >(const Token& lhs, const Token::Type& rhs);
		friend bool operator <(const Token& lhs, const Token::Type& rhs);
		friend bool operator>=(const Token& lhs, const Token::Type& rhs);
		friend bool operator<=(const Token& lhs, const Token::Type& rhs);

		friend bool operator==(const Token& lhs, const char& rhs);
		friend bool operator!=(const Token& lhs, const char& rhs);
		friend bool operator >(const Token& lhs, const char& rhs);
		friend bool operator <(const Token& lhs, const char& rhs);
		friend bool operator>=(const Token& lhs, const char& rhs);
		friend bool operator<=(const Token& lhs, const char& rhs);

		friend bool operator==(const Token& lhs, const std::string& rhs);
		friend bool operator!=(const Token& lhs, const std::string& rhs);
	};
}

#endif // !_TOKEN_H_
