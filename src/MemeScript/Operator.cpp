#include <MemeScript/Operator.h>
#include <MemeCore/DebugUtility.h>

namespace ml
{
	//const std::string Operator::OpCodes[Operator::MAX_OP_TYPE] =
	const Operator::OperMap Operator::OpValues = {
	{ "=",	OperatorType::OP_SET },
	{ "^=",	OperatorType::OP_POW_SET },
	{ "*=",	OperatorType::OP_MUL_SET },
	{ "/=",	OperatorType::OP_DIV_SET },
	{ "%=",	OperatorType::OP_DIV_SET },
	{ "+=",	OperatorType::OP_ADD_SET },
	{ "-=",	OperatorType::OP_SUB_SET },

	{ "==", OperatorType::OP_EQU },
	{ "!=", OperatorType::OP_NEQ },
	{ "<=", OperatorType::OP_LTE },
	{ ">=", OperatorType::OP_GTE },
	{ "&&", OperatorType::OP_AND },
	{ "||", OperatorType::OP_OR  },
	{ "<",  OperatorType::OP_LT  },
	{ ">",  OperatorType::OP_GT  },

	{ "^",  OperatorType::OP_POW },
	{ "*",  OperatorType::OP_MUL },
	{ "/",  OperatorType::OP_DIV },
	{ "%",  OperatorType::OP_MOD },
	{ "+",  OperatorType::OP_ADD },
	{ "-",  OperatorType::OP_SUB },
	};

	Operator::Operator()
		: type(OperatorType::OP_INVALID)
	{
	}

	Operator::Operator(OperatorType type)
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
		op = OperatorType::OP_INVALID;
		return false;
	}


	bool Operator::equals(const Operator & value) const
	{
		return type == value.type;
	}

	bool Operator::equals(const OperatorType & value) const
	{
		return type == value;
	}

	bool Operator::lessThan(const Operator & value) const
	{
		return type < value.type;
	}

	bool Operator::lessThan(const OperatorType & value) const
	{
		return type < value;
	}

	void Operator::serialize(std::ostream & out) const
	{
		out << (FG::Black | BG::Cyan) << type << FMT();
	}
}