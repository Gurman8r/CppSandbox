#include <MemeScript/Token.hpp>
#include <MemeScript/Operator.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	const Token::NameMap Token::TypeNames({
		{ TOK_ERR,	"ERR" },
		{ TOK_VOID, "VOID" },
		{ TOK_ENDL, "ENDL" },

		{ TOK_NAME, "NAME" },
		{ TOK_FLT,	"FLT" },
		{ TOK_INT,	"INT" },
		{ TOK_STR,	"STR" },

		{ TOK_POW,	"POW" },
		{ TOK_MUL,	"MUL" },
		{ TOK_DIV,	"DIV" },
		{ TOK_ADD,	"ADD" },
		{ TOK_SUB,	"SUB" },

		{ TOK_EQU,	"EQU" },
		{ TOK_LT,	"LT" },
		{ TOK_GT,	"GT" },
		{ TOK_NOT,  "NOT" },

		{ TOK_HASH, "HASH" },
		{ TOK_TERM, "TERM" },
		{ TOK_SEMI, "SEMI" },
		{ TOK_CMMA, "CMMA" },
		{ TOK_COLN, "COLN" },

		{ TOK_LPRN, "LPRN" },{ TOK_RPRN, "RPRN" } , // (paren)
		{ TOK_LBRC, "LBRC" },{ TOK_RBRC, "RBRC" } , // {brace}
		{ TOK_LBKT, "LBKT" },{ TOK_RBKT, "RBKT" } , // [bracket]

		{ TOK_DQTE, "DQTE" },{ TOK_SQTE, "SQTE" },

		{ TOK_USCR, "USCR" },
		{ TOK_TNRY, "TNRY" },
		});

	const Token::SymbolMap Token::Symbols({
		{ "ERR",  TOK_ERR },
		{ "VOID", TOK_VOID },
		{ "ENDL", TOK_ENDL },

		{ "^", TOK_POW },
		{ "*", TOK_MUL },
		{ "/", TOK_DIV },
		{ "+", TOK_ADD },
		{ "-", TOK_SUB },
		{ "%", TOK_MOD },

		{ "&", TOK_AND },
		{ "|", TOK_OR  },

		{ "=", TOK_EQU },
		{ "<", TOK_LT  },
		{ ">", TOK_GT  },
		{ "!", TOK_NOT },

		{ ";", TOK_SEMI },
		{ ",", TOK_CMMA },
		{ ":", TOK_COLN },
		{ "#", TOK_HASH },
		{ "\\",TOK_TERM },

		{ "(", TOK_LPRN },{ ")", TOK_RPRN },
		{ "{", TOK_LBRC },{ "}", TOK_RBRC },
		{ "[", TOK_LBKT },{ "]", TOK_RBKT },

		{ "\"", TOK_DQTE }, { "\'", TOK_SQTE},

		{ "_", TOK_USCR },
		{ "_", TOK_TNRY },

		});


	Token::Token()
		: type(TOK_ERR)
		, data(Token::TypeNames.at(type))
	{
	}

	Token::Token(TokenType type)
		: type(type)
		, data(Token::TypeNames.at(type))
	{
	}

	Token::Token(char type)
		: type((TokenType)type)
		, data(String())
	{
	}

	Token::Token(TokenType type, const String & data)
		: type(type)
		, data(data)
	{
	}

	Token::Token(char type, const String & data)
		: type((TokenType)type)
		, data(data)
	{
	}

	Token::Token(const Token & copy)
		: type(copy.type)
		, data(copy.data)
	{
	}


	Token & Token::operator=(const Token & value)
	{
		type = value.type;
		data = value.data;
		return (*this);
	}

	Token & Token::operator=(const TokenType & value)
	{
		type = value;
		data = Token::TypeNames.at(value);
		return (*this);
	}


	bool Token::isOperand() const
	{
		return
			type == 'n' ||
			type == 'f' ||
			type == 'i' ||
			type == 's';
	}

	bool Token::isOperator() const
	{
		return
			type == TOK_POW ||
			type == TOK_MUL ||
			type == TOK_DIV ||
			type == TOK_ADD ||
			type == TOK_SUB ||
			type == TOK_EQU ||
			type == TOK_LT ||
			type == TOK_GT ||
			type == TOK_NOT;
	}

	
	bool Token::equals(const Token & value) const
	{
		return (type == value.type) && (data == value.data);
	}

	bool Token::equals(const TokenType & value) const
	{
		return type == value;
	}

	bool Token::equals(const char & value) const
	{
		return (char)type == value;
	}

	bool Token::equals(const String & value) const
	{
		return data == value;
	}
	
	
	bool Token::lessThan(const Token & value) const
	{
		Operator lo;
		if (Operator::makeOperator(data, lo))
		{
			Operator ro;
			if (Operator::makeOperator(value.data, ro))
			{
				return lo < ro;
			}
		}
		return false;
	}

	bool Token::lessThan(const TokenType & value) const
	{
		return type < value;
	}
	
	bool Token::lessThan(const char & value) const
	{
		return (char)type < value;
	}

	bool Token::lessThan(const String & value) const
	{
		return data < value;
	}


	void Token::serialize(std::ostream & out) const
	{
		out << FMT()
			<< FG::White << "[ "
			<< FG::Green << type
			<< FG::White << " "
			<< FG::Yellow << data
			<< FG::White << " ]"
			<< FMT();
	}
}