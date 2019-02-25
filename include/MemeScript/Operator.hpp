#ifndef _OPERATOR_HPP_
#define _OPERATOR_HPP_

#include <MemeScript/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	enum OpType : int32_t
	{
		OP_INVALID = -1,

		OP_SET,		//  =
		OP_POW_SET,	// ^=
		OP_MUL_SET,	// *=
		OP_DIV_SET,	// /=
		OP_MOD_SET,	// %=
		OP_ADD_SET,	// +=
		OP_SUB_SET,	// -=

		OP_EQU,		// ==
		OP_NEQ,		// !=
		OP_LT,		//  <
		OP_GT,		//  >
		OP_LTE,		// <=
		OP_GTE,		// >=
		OP_AND,		// &&
		OP_OR,		// ||

		OP_POW,		//  ^
		OP_MUL,		//  *
		OP_DIV,		//  /
		OP_MOD,		//  %
		OP_ADD,		//  +
		OP_SUB,		//  -
	};

	struct ML_SCRIPT_API Operator final
		: public ITrackable
		, public IComparable<Operator>
		, public IComparable<OpType>
	{
		using OperMap = HashMap<String, OpType>;

		static const HashMap<String, OpType> OpValues;

		Operator();
		Operator(OpType type);
		Operator(const Operator & copy);
		~Operator();

		OpType type;

		static bool makeOperator(const String & str, Operator & op);
		static bool makeOperator(const String & a, const String & b, Operator & op);

		bool equals(const Operator & value) const override;
		bool equals(const OpType & value) const override;

		bool lessThan(const Operator & value) const override;
		bool lessThan(const OpType & value) const override;

		void serialize(std::ostream & out) const override;
	};

	inline std::ostream & operator<<(std::ostream & out, const OpType & rhs)
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
}

#endif // !_OPERATOR_HPP_
