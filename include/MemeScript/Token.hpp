#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <MemeScript/Args.hpp>

namespace ml
{
	class ML_SCRIPT_API Token final
		: public ITrackable
		, public IComparable<Token>
		, public IComparable<char>
		, public IComparable<String>
	{
	public:
		enum : char
		{
			T_Error		= '\0', // '\0' | Error
			T_Void		= ' ',	// '  ' | Nothing / Space
			T_Endl		= '\n',	// '\n' | End of line
			T_Name		= 'n',	// 'n'	| Name
			T_Float		= 'f',	// 'f'	| Float / Decimal
			T_Int		= 'i',	// 'i'	| Integer
			T_String	= 's',	// 's'	| String
			T_Pow		= '^',	// '^'	| Pow
			T_Mul		= '*',	// '*'	| Multiply
			T_Div		= '/',	// '/'	| Divide
			T_Plus		= '+',	// '+'	| Add
			T_Minus		= '-',	// '-'	| Subtract
			T_Equal		= '=',	// '='	| Equal
			T_Less		= '<',	// '<'	| Less Than
			T_Greater	= '>',	// '>'	| Greater Than
			T_Not		= '!',	// '!'	| Not
			T_Mod		= '%',	// '%'	| Mod
			T_Or		= '|',	// '|'	| Or
			T_And		= '&',	// '&'	| And
			T_Hash		= '#',	// '#'	| Hash
			T_Term		= '\\',	// '\\' | Terminate
			T_SColon	= ';',	// ';'	| Semicolon
			T_Comma		= ',',	// ','	| Comma
			T_Colon		= ':',	// ':'	| Colon
			T_Dot		= '.',	// '.'	| Dot
			T_QMark		= '?',	// '?'	| Ternary
			T_Dollar	= '$',	// '$'	| Dollar
			T_At		= '@',	// '@'	| At
			T_LParen	= '(',	// '('	| Left Paren
			T_RParen	= ')',	// ')'	| Right Paren
			T_LBrace	= '{',	// '{'	| Left Brace
			T_RBrace	= '}',	// '}'	| Right Brace
			T_LBrack	= '[',	// '['	| Left Bracket
			T_RBrack	= ']',	// ']'	| Right Bracket
			T_DQuote	= '\"', // '\"' | Double Quote
			T_SQuote	= '\'', // '\'' | Single Quote
		};

	public:
		using SymbolMap = HashMap<String, char>;
		using NameMap = HashMap<char, String>;

		static const NameMap	TypeNames;
		static const SymbolMap	Symbols;

		char	type;
		String	data;

		Token();
		Token(char type);
		Token(char type, const String & data);
		Token(const Token & copy);

		Token & operator=(const Token & value);
		Token & operator=(const char & value);

		bool isOperand() const;
		bool isOperator() const;

		bool equals(const Token & value) const override;
		bool equals(const char & value) const override;
		bool equals(const String & value) const override;

		bool lessThan(const Token & value) const override;
		bool lessThan(const char & value) const override;
		bool lessThan(const String & value) const override;

		void serialize(std::ostream & out) const override;
	};
}

#endif // !_TOKEN_HPP_
