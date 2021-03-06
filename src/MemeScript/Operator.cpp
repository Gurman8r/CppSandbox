#include <MemeScript/Operator.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const HashMap<String, int32_t> Operator::Names = {
		{	" "	,	 OP_INVALID	},
		{	"="	,	 OP_SET		},
		{	"^=",	 OP_POW_SET	},
		{	"*=",	 OP_MUL_SET	},
		{	"/=",	 OP_DIV_SET	},
		{	"%=",	 OP_MOD_SET	},
		{	"+=",	 OP_ADD_SET	},
		{	"-=",	 OP_SUB_SET	},
		{	"==",	 OP_EQU		},
		{	"!=",	 OP_NEQ		},
		{	"<"	,	 OP_LT		},
		{	">"	,	 OP_GT		},
		{	"<=",	 OP_LTE		},
		{	">=",	 OP_GTE		},
		{	"&&",	 OP_AND		},
		{	"||",	 OP_OR		},
		{	"^"	,	 OP_POW		},
		{	"*"	,	 OP_MUL		},
		{	"/"	,	 OP_DIV		},
		{	"%"	,	 OP_MOD		},
		{	"+"	,	 OP_ADD		},
		{	"-"	,	 OP_SUB		},
	};

	/* * * * * * * * * * * * * * * * * * * * */

	Operator::Operator(int32_t type)
		: type(type)
	{
	}

	Operator::Operator(const String & text)
		: Operator(OP_INVALID)
	{
		const_iterator it;
		if ((it = Names.find(text)) != Names.end())
		{
			type = it->second;
		}
	}

	Operator::Operator(const String & lhs, const String & rhs)
		: Operator(lhs + rhs)
	{
	}

	Operator::Operator(const Operator & copy)
		: type(copy.type)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Operator::equals(const Operator & value) const
	{
		return type == value.type;
	}

	bool Operator::equals(const int32_t & value) const
	{
		return type == value;
	}


	bool Operator::lessThan(const Operator & value) const
	{
		return type < value.type;
	}

	bool Operator::lessThan(const int32_t & value) const
	{
		return type < value;
	}


	void Operator::serialize(std::ostream & out) const
	{
		auto it = std::find_if(Names.begin(), Names.end(), [&](auto && pair)
		{
			return (pair.second == type);
		});
		out << (FG::Black | BG::Cyan) 
			<< ((it != Names.end())
				? it->first
				: String(" "))
			<< FMT();
	}

	void Operator::deserialize(std::istream & in)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}