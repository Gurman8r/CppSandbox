#include <MemeScript/Operator.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	//const std::string Operator::OpCodes[Operator::MAX_OP_TYPE] =
	const Operator::OperMap Operator::OpValues = {
	{ "=",	Operator::OP_SET },
	{ "^=",	Operator::OP_POW_SET },
	{ "*=",	Operator::OP_MUL_SET },
	{ "/=",	Operator::OP_DIV_SET },
	{ "%=",	Operator::OP_DIV_SET },
	{ "+=",	Operator::OP_ADD_SET },
	{ "-=",	Operator::OP_SUB_SET },

	{ "==", Operator::OP_EQU },
	{ "!=", Operator::OP_NEQ },
	{ "<=", Operator::OP_LTE },
	{ ">=", Operator::OP_GTE },
	{ "&&", Operator::OP_AND },
	{ "||", Operator::OP_OR  },
	{ "<",  Operator::OP_LT  },
	{ ">",  Operator::OP_GT  },

	{ "^",  Operator::OP_POW },
	{ "*",  Operator::OP_MUL },
	{ "/",  Operator::OP_DIV },
	{ "%",  Operator::OP_DIV },
	{ "+",  Operator::OP_ADD },
	{ "-",  Operator::OP_SUB },
	};

	Operator::Operator()
		: type(Operator::OP_INVALID)
	{
	}

	Operator::Operator(Operator::Type type)
		: type(type)
	{
	}

	Operator::Operator(const Operator & copy)
		: type(copy.type)
	{
	}

	Operator::~Operator()
	{
	}


	bool Operator::makeOperator(const std::string& str, Operator& op)
	{
		Operator::OperMap::const_iterator it = OpValues.find(str);
		if (it != OpValues.end())
		{
			op = Operator(it->second);
			return true;
		}
		op = Operator::OP_INVALID;
		return false;
	}


	std::ostream & operator<<(std::ostream & out, const Operator::Type & rhs)
	{
		for (auto pair : Operator::OpValues)
		{
			if (pair.second == rhs)
			{
				out << pair.first;
				break;
			}
		}
		return out;
	}

	std::ostream & operator<<(std::ostream & out, const Operator & rhs)
	{
		out << (FG::Black | BG::Cyan) << rhs.type << FMT();
		return out;
	}


	bool operator==(const Operator & lhs, const Operator & rhs)
	{
		return lhs.type == rhs.type;
	}

	bool operator!=(const Operator & lhs, const Operator & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator>(const Operator & lhs, const Operator & rhs)
	{
		return lhs.type > rhs.type;
	}

	bool operator<(const Operator & lhs, const Operator & rhs)
	{
		return lhs.type < rhs.type;
	}

	bool operator>=(const Operator & lhs, const Operator & rhs)
	{
		return lhs > rhs || lhs == rhs;
	}

	bool operator<=(const Operator & lhs, const Operator & rhs)
	{
		return lhs < rhs || lhs == rhs;
	}


	bool operator==(const Operator & lhs, const Operator::Type & rhs)
	{
		return lhs.type == rhs;
	}

	bool operator!=(const Operator & lhs, const Operator::Type & rhs)
	{
		return !(lhs == rhs);
	}

	bool operator>(const Operator & lhs, const Operator::Type & rhs)
	{
		return lhs.type > rhs;
	}

	bool operator<(const Operator & lhs, const Operator::Type & rhs)
	{
		return lhs.type < rhs;
	}

	bool operator>=(const Operator & lhs, const Operator::Type & rhs)
	{
		return lhs > rhs || lhs == rhs;
	}

	bool operator<=(const Operator & lhs, const Operator::Type & rhs)
	{
		return lhs < rhs || lhs == rhs;
	}
}