#include <MemeScript/Operator.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	const Operator::OperMap Operator::OpValues = {
		{ "=",	OpType::OP_SET },
		{ "^=",	OpType::OP_POW_SET },
		{ "*=",	OpType::OP_MUL_SET },
		{ "/=",	OpType::OP_DIV_SET },
		{ "%=",	OpType::OP_DIV_SET },
		{ "+=",	OpType::OP_ADD_SET },
		{ "-=",	OpType::OP_SUB_SET },

		{ "==", OpType::OP_EQU },
		{ "!=", OpType::OP_NEQ },
		{ "<=", OpType::OP_LTE },
		{ ">=", OpType::OP_GTE },
		{ "&&", OpType::OP_AND },
		{ "||", OpType::OP_OR  },
		{ "<",  OpType::OP_LT  },
		{ ">",  OpType::OP_GT  },

		{ "^",  OpType::OP_POW },
		{ "*",  OpType::OP_MUL },
		{ "/",  OpType::OP_DIV },
		{ "%",  OpType::OP_MOD },
		{ "+",  OpType::OP_ADD },
		{ "-",  OpType::OP_SUB },
	};

	Operator::Operator()
		: type(OpType::OP_INVALID)
	{
	}

	Operator::Operator(OpType type)
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


	bool Operator::makeOperator(const String & str, Operator & op)
	{
		Operator::OperMap::const_iterator it = OpValues.find(str);
		if (it != OpValues.end())
		{
			op = Operator(it->second);
			return true;
		}
		op = OpType::OP_INVALID;
		return false;
	}

	bool Operator::makeOperator(const String & a, const String & b, Operator & op)
	{
		return makeOperator(a + b, op);
	}


	bool Operator::equals(const Operator & value) const
	{
		return type == value.type;
	}

	bool Operator::equals(const OpType & value) const
	{
		return type == value;
	}

	bool Operator::lessThan(const Operator & value) const
	{
		return type < value.type;
	}

	bool Operator::lessThan(const OpType & value) const
	{
		return type < value;
	}

	void Operator::serialize(std::ostream & out) const
	{
		out << (FG::Black | BG::Cyan) << type << FMT();
	}
}