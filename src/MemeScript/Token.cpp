#include <MemeScript/Token.hpp>
#include <MemeScript/Operator.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	const Token::NameMap Token::TypeNames({
		{ T_Error,		"ERR"	},
		{ T_Void,		"VOID"	},
		{ T_Endl,		"ENDL"	},
		{ T_Name,		"NAME"	},
		{ T_Float,		"FLT"	},
		{ T_Int,		"INT"	},
		{ T_String,		"STR"	},
		{ T_Pow,		"POW"	},
		{ T_Mul,		"MUL"	},
		{ T_Div,		"DIV"	},
		{ T_Plus,		"ADD"	},
		{ T_Minus,		"SUB"	},
		{ T_Mod,		"MOD"	},
		{ T_Equal,		"EQU"	},
		{ T_Less,		"LT"	},
		{ T_Greater,	"GT"	},
		{ T_Not,		"NOT"	},
		{ T_Hash,		"HASH"	},
		{ T_Term,		"TERM"	},
		{ T_SColon,		"SCOL"	},
		{ T_Comma,		"CMMA"	},
		{ T_Colon,		"COLN"	},
		{ T_Dot,		"DOT"	},
		{ T_QMark,		"QMRK"	},
		{ T_Dollar,		"DOLR"	},
		{ T_At,			"AT"	},
		{ T_LParen,		"LPRN"	},
		{ T_RParen,		"RPRN"	},
		{ T_LBrace,		"LBRC"	},
		{ T_RBrace,		"RBRC"	},
		{ T_LBrack,		"LBKT"	},
		{ T_RBrack,		"RBKT"	},
		{ T_DQuote,		"DQTE"	},
		{ T_SQuote,		"SQTE"	},
	});

	const Token::SymbolMap Token::Symbols({
		{ "ERR",	T_Error		},
		{ "VOID",	T_Void		},
		{ "ENDL",	T_Endl		},
		{ "^",		T_Pow		},
		{ "*",		T_Mul		},
		{ "/",		T_Div		},
		{ "+",		T_Plus		},
		{ "-",		T_Minus		},
		{ "%",		T_Mod		},
		{ "&",		T_And		},
		{ "|",		T_Or		},
		{ "=",		T_Equal		},
		{ "<",		T_Less		},
		{ ">",		T_Greater	},
		{ "!",		T_Not		},
		{ ";",		T_SColon	},
		{ ",",		T_Comma		},
		{ ":",		T_Colon		},
		{ "#",		T_Hash		},
		{ "\\",		T_Term		},
		{ ".",		T_Dot		},
		{ "?",		T_QMark		},
		{ "$",		T_Dollar	},
		{ "@",		T_At		},
		{ "(",		T_LParen	}, 
		{ ")",		T_RParen	},
		{ "{",		T_LBrace	}, 
		{ "}",		T_RBrace	},
		{ "[",		T_LBrack	}, 
		{ "]",		T_RBrack	},
		{ "\"",		T_DQuote	},
		{ "\'",		T_SQuote	},

	});


	Token::Token()
		: type(T_Error)
		, data(Token::TypeNames.at(type))
	{
	}

	Token::Token(char type)
		: type(type)
		, data(String())
	{
	}

	Token::Token(char type, const String & data)
		: type(type)
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

	Token & Token::operator=(const char & value)
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
			type == '^' ||
			type == '*' ||
			type == '/' ||
			type == '+' ||
			type == '-' ||
			type == '=' ||
			type == '<' ||
			type == '>' ||
			type == '!' ||
			type == '%' ||
			type == '|' ||
			type == '&';
	}

	
	bool Token::equals(const Token & value) const
	{
		return (type == value.type) && (data == value.data);
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
			<< FG::Green << TypeNames.at(type)
			<< FG::White << " "
			<< FG::Yellow << data
			<< FG::White << " ]"
			<< FMT();
	}
}