#include <MemeScript/Token.hpp>
#include <MemeScript/Operator.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	const Token::SymbolMap Token::Symbols({
		{ "\0",		T_Error		},
		{ " ",		T_Void		},
		{ "\n",		T_Endl		},
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
		, data(String(1, type))
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
		data = String(1, type);
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
		if (Operator lhs = Operator(data))
		{
			if (Operator rhs = Operator(value.data))
			{
				return lhs < rhs;
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
			<< FG::Green << type
			<< FG::White << ", "
			<< FG::Yellow << data
			<< FG::White << " ]"
			<< FMT();
	}
}