#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include <MemeScript/Export.h>
#include <MemeCore/ITrackable.h>
#include <map>

namespace ml
{
	struct ML_SCRIPT_API Operator final
		: public ITrackable
	{
		enum Type : int
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

			MAX_OP_TYPE
		};

		using OperMap = std::map<std::string, Operator::Type>;
		static const std::map<std::string, Type> OpValues;

		Operator();
		Operator(Operator::Type type);
		Operator(const Operator& copy);
		~Operator();

		Type type;

		static bool makeOperator(const std::string& str, Operator& op);

		friend std::ostream& operator<<(std::ostream& out, const Operator::Type& rhs);
		friend std::ostream& operator<<(std::ostream& out, const Operator& rhs);

		friend bool operator==(const Operator& lhs, const Operator& rhs);
		friend bool operator!=(const Operator& lhs, const Operator& rhs);
		friend bool operator >(const Operator& lhs, const Operator& rhs);
		friend bool operator <(const Operator& lhs, const Operator& rhs);
		friend bool operator>=(const Operator& lhs, const Operator& rhs);
		friend bool operator<=(const Operator& lhs, const Operator& rhs);

		friend bool operator==(const Operator& lhs, const Operator::Type& rhs);
		friend bool operator!=(const Operator& lhs, const Operator::Type& rhs);
		friend bool operator >(const Operator& lhs, const Operator::Type& rhs);
		friend bool operator <(const Operator& lhs, const Operator::Type& rhs);
		friend bool operator>=(const Operator& lhs, const Operator::Type& rhs);
		friend bool operator<=(const Operator& lhs, const Operator::Type& rhs);
	};
}

#endif // !_OPERATOR_H_
