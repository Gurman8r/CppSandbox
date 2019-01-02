#include <MemeScript/Token.h>
#include <MemeScript/Operator.h>
#include <MemeCore/ConsoleUtility.h>

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
		});


	Token::Token()
		: type(TOK_ERR)
		, data(Token::TypeNames.at(type))
	{
	}

	Token::Token(Type type)
		: type(type)
		, data(Token::TypeNames.at(type))
	{
	}

	Token::Token(char type)
		: type((Token::Type)type)
		, data(std::string())
	{
	}

	Token::Token(Type type, const std::string & data)
		: type(type)
		, data(data)
	{
	}

	Token::Token(char type, const std::string & data)
		: type((Token::Type)type)
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

	Token & Token::operator=(const Token::Type & value)
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

	std::string Token::str() const
	{
		std::stringstream ss;
		ss << (*this);
		return ss.str();
	}


	std::ostream & operator<<(std::ostream & out, const Token::Type & rhs)
	{
		out << Token::TypeNames.at(rhs);
		return out;
	}

	std::ostream & operator<<(std::ostream & out, const Token & tok)
	{
		out
			<< FMT()
			<< FG::White << "[ "
			<< FG::Green << tok.type
			<< FG::White << " "
			<< FG::Yellow << tok.data
			<< FG::White << " ]" 
			<< FMT();
		return out;
	}


	bool operator==(const Token & lhs, const Token & rhs)
	{
		return
			(lhs.type == rhs.type) &&
			(lhs.data == rhs.data);
	}

	bool operator!=(const Token & lhs, const Token & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator >(const Token & lhs, const Token & rhs)
	{
		return (lhs.type > rhs.type);
	}

	bool operator <(const Token & lhs, const Token & rhs)
	{
		Operator lo;
		if (Operator::makeOperator(lhs.data, lo))
		{
			Operator ro;
			if (Operator::makeOperator(rhs.data, ro))
			{
				return lo < ro;
			}
		}

		return false;
	}

	bool operator>=(const Token & lhs, const Token & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	bool operator<=(const Token & lhs, const Token & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}


	bool operator==(const Token & lhs, const Token::Type & rhs)
	{
		return lhs.type == rhs;
	}

	bool operator!=(const Token & lhs, const Token::Type & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator >(const Token & lhs, const Token::Type & rhs)
	{
		return lhs.type > rhs;
	}

	bool operator <(const Token & lhs, const Token::Type & rhs)
	{
		return lhs.type < rhs;
	}

	bool operator>=(const Token & lhs, const Token::Type & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	bool operator<=(const Token & lhs, const Token::Type & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}


	bool operator==(const Token & lhs, const char & rhs)
	{
		return (char)lhs.type == rhs;
	}

	bool operator!=(const Token & lhs, const char & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator >(const Token & lhs, const char & rhs)
	{
		return (char)lhs.type > rhs;
	}

	bool operator <(const Token & lhs, const char & rhs)
	{
		return (char)lhs.type < rhs;
	}

	bool operator>=(const Token & lhs, const char & rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	}

	bool operator<=(const Token & lhs, const char & rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	}


	bool operator==(const Token & lhs, const std::string & rhs)
	{
		return (lhs.data == rhs);
	}

	bool operator!=(const Token & lhs, const std::string & rhs)
	{
		return !(lhs == rhs);
	}

}