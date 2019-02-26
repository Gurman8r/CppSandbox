#ifndef _OPERATOR_HPP_
#define _OPERATOR_HPP_

#include <MemeScript/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_SCRIPT_API Operator final
		: public ITrackable
		, public IComparable<Operator>
		, public IComparable<int32_t>
	{
	public:
		enum : int32_t
		{
			OP_INVALID	= 0x00,	//	' '	
			OP_SET		= 0x01,	//	'='	
			OP_POW_SET	= 0x02,	//	'^='	
			OP_MUL_SET	= 0x03,	//	'*='	
			OP_DIV_SET	= 0x04,	//	'/='	
			OP_MOD_SET	= 0x05,	//	'%='	
			OP_ADD_SET	= 0x06,	//	'+='	
			OP_SUB_SET	= 0x07,	//	'-='	
			OP_EQU		= 0x08,	//	'=='	
			OP_NEQ		= 0x09,	//	'!='	
			OP_LT		= 0x0A,	//	'<'	
			OP_GT		= 0x0B,	//	'>'	
			OP_LTE		= 0x0C,	//	'<='	
			OP_GTE		= 0x0D,	//	'>='	
			OP_AND		= 0x0E,	//	'&&'	
			OP_OR		= 0x0F,	//	'||'	
			OP_POW		= 0x10,	//	'^'	
			OP_MUL		= 0x20,	//	'*'	
			OP_DIV		= 0x30,	//	'/'	
			OP_MOD		= 0x40,	//	'%'	
			OP_ADD		= 0x50,	//	'+'	
			OP_SUB		= 0x60,	//	'-'	
		};
	public:
		using const_iterator = typename HashMap<String, int32_t>::const_iterator;

		static const HashMap<String, int32_t> Names;

	public:
		Operator();
		Operator(int32_t type);
		Operator(const String & text);
		Operator(const String & lhs, const String & rhs);
		Operator(const Operator & copy);
		~Operator() {}

	public:
		int32_t type;

		inline operator bool() const { return (type != OP_INVALID); }

		String getName() const;

	public:
		bool equals(const Operator & value) const override;
		bool equals(const int32_t & value) const override;

		bool lessThan(const Operator & value) const override;
		bool lessThan(const int32_t & value) const override;

		void serialize(std::ostream & out) const override;
	};
}

#endif // !_OPERATOR_HPP_
