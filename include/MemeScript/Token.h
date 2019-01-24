#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <MemeScript/Args.h>
#include <unordered_map>

namespace ml
{
	enum TokenType : char
	{
		TOK_ERR = '\0', // \0 - Invalid / Null
		TOK_VOID = ' ',	// ' ' - Empty / Space
		TOK_ENDL = '\n',// \n - End of line

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

		TOK_USCR = '_',	// _ - Underscore
		TOK_TNRY = '?', // ? - Ternary

		MAX_TOKEN_TYPE
	};

	struct ML_SCRIPT_API Token final
		: public ITrackable
		, public IComparable<Token>
		, public IComparable<char>
		, public IComparable<TokenType>
		, public IComparable<std::string>
	{
		using SymbolMap = std::unordered_map<std::string, TokenType>;
		using NameMap = std::unordered_map<TokenType, std::string>;

		static const NameMap	TypeNames;
		static const SymbolMap	Symbols;

		TokenType		type;
		std::string		data;

		Token();
		Token(TokenType type);
		Token(char type);
		Token(TokenType type, const std::string & data);
		Token(char type, const std::string & data);
		Token(const Token & copy);

		Token & operator=(const Token & value);
		Token & operator=(const TokenType & value);

		bool isOperand() const;
		bool isOperator() const;

		bool equals(const Token & value) const override;
		bool equals(const TokenType & value) const override;
		bool equals(const char & value) const override;
		bool equals(const std::string & value) const override;

		bool lessThan(const Token & value) const override;
		bool lessThan(const TokenType & value) const override;
		bool lessThan(const char & value) const override;
		bool lessThan(const std::string & value) const override;

		void serialize(std::ostream & out) const override;
	};

	inline std::ostream& operator<<(std::ostream& out, const TokenType& rhs)
	{
		return out << Token::TypeNames.at(rhs);
	}
}

#endif // !_TOKEN_H_
