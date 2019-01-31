#ifndef _VAR_H_
#define _VAR_H_

#include <MemeCore/IComparable.hpp>
#include <MemeScript/TokenList.hpp>
#include <MemeScript/StringUtility.hpp>
#include <iostream>

namespace ml
{
	class ML_SCRIPT_API Var final
		: public ITrackable
		, public IComparable<Var>
	{
	public:
		struct ML_SCRIPT_API Ptr
			: public ITrackable
		{
			String		name;
			int32_t		index;

			Ptr();
			Ptr(int32_t index, const String& name);
			Ptr(const Ptr& copy);

			Var * get() const;

			void serialize(std::ostream & out) const override;

			inline Var * operator->() const { return get(); }
			inline Var * operator*() const { return get(); }
		};

	public:
		enum Type : int32_t
		{
			INVALID_TYPE = -1,

			//- Basic - - -//
			MIN_BASIC,
			Void = MIN_BASIC,
			Bool,
			Float,
			Integer,
			Pointer,
			Str,
			MAX_BASIC,

			//- Complex - -//
			MIN_COMPLEX,
			Array = MIN_COMPLEX,
			Func,
			MAX_COMPLEX,

			//- - - - - - -//
			MAX_VAR_TYPE
		};
		
		static const String TypeNames[Type::MAX_VAR_TYPE];
		
		inline friend std::ostream & operator<<(std::ostream & out, const Type & rhs)
		{
			return out << Var::TypeNames[rhs];;
		}


	public:
		Var();
		Var(const Type& type, const TokenList& data);
		Var(const Var & other);
		~Var();

	public:
		inline int32_t	getScope()		const { return m_scope; }
		inline Type		getType()		const { return m_type; }
		inline String	getTypeName()	const { return TypeNames[getType()]; }

		inline Var & setScope(int32_t value)
		{
			m_scope = value;
			return (*this);
		}
		inline Var & setType(const Type & value)
		{
			m_changed = !compareType(value);
			m_type = value;
			return (*this);
		}

	public: // Check Functions

		bool	compareTokens(const TokenList& value) const;
		bool	compareType(const Type& type) const;

		bool	isValid() const;
		bool	isArrayType() const;
		bool	isBasicType() const;
		bool	isBoolType() const;
		bool	isComplexType() const;
		bool	isEmptyValue() const;
		bool	isErrorType() const;
		bool	isFloatType() const;
		bool	isFuncType() const;
		bool	isIntType() const;
		bool	isNameType() const;
		bool	isNullValue() const;
		bool	isNumericType() const;
		bool	isPointerType() const;
		bool	isStringType() const;
		bool	isVoidType() const;

		inline operator bool() const { return !isErrorType(); }

	public: // Data Functions
		bool		boolValue() const;
		float		floatValue() const;
		Var			elemValue(size_t i) const;
		String		errorValue() const;
		size_t		intValue() const;
		Ptr			pointerValue() const;
		size_t		sizeOfValue() const;
		String		stringValue() const;
		String		textValue() const;
		TokenList	tokensValue() const;

		Var & arrayValue(const TokenList & value);
		Var & boolValue(const bool & value);
		Var & elemValue(size_t index, const Token & value);
		Var & errorValue(const String & value);
		Var & floatValue(const float & value);
		Var & funcValue(const TokenList & value);
		Var & intValue(const size_t & value);
		Var & nullValue();
		Var & pointerValue(const Ptr & value);
		Var & stringValue(const String & value);
		Var & tokensValue(const TokenList & value);
		Var & voidValue();
		
		template<typename T, typename ... A>
		inline Var & errorValue(const String & fmt, const T & arg0, const A &... args)
		{
			return errorValue(String::Format(fmt, arg0, (args)...));
		};


	public: // Serialization
		void serialize(std::ostream & out) const override;

		static std::ostream& PrintList(std::ostream & out, const Var & value);
		
	public: // Factory
		static Var makeSingle(const Token & tok);
		static Var makeRecursive(const TokenList & toks);

	public: // Operator Functions
		bool	And(const Var & other) const;	// &&
		bool	Or(const Var & other) const;	// ||

		bool	Equals(const Var & other) const;// ==
		bool	Less(const Var & other) const;	// <

		Var &	Add(const Var & other); // +
		Var &	Div(const Var & other); // -
		Var &	Mod(const Var & other); // %
		Var &	Mul(const Var & other); // *
		Var &	Pow(const Var & other); // ^
		Var &	Sub(const Var & other); // -
		Var &	Set(const Var & other); // =


	public: // IComparable
		inline bool equals(const Var & other)	const override { return Equals(other); }
		inline bool lessThan(const Var & other)	const override { return Less(other); }

	public: // Operators
		friend bool operator&&(const Var & lhs, const Var & rhs);
		friend bool operator||(const Var & lhs, const Var & rhs);

		friend Var & operator+=(Var & lhs, const Var & rhs);
		friend Var & operator-=(Var & lhs, const Var & rhs);
		friend Var & operator*=(Var & lhs, const Var & rhs);
		friend Var & operator/=(Var & lhs, const Var & rhs);
		friend Var & operator^=(Var & lhs, const Var & rhs);
		friend Var & operator%=(Var & lhs, const Var & rhs);

		friend Var	operator+(const Var & lhs, const Var & rhs);
		friend Var	operator-(const Var & lhs, const Var & rhs);
		friend Var	operator*(const Var & lhs, const Var & rhs);
		friend Var	operator/(const Var & lhs, const Var & rhs);
		friend Var	operator^(const Var & lhs, const Var & rhs);
		friend Var	operator%(const Var & lhs, const Var & rhs);

		Var & operator=(const Var & rhs);
		Var & operator=(bool value);
		Var & operator=(float value);
		Var & operator=(double value);
		Var & operator=(int32_t value);
		Var & operator=(const Ptr & value);
		Var & operator=(const String & value);
		Var & operator=(const char * value);
		Var & operator=(char value);

	private:
		TokenList	m_tokens;
		int32_t		m_scope;
		Type		m_type;

		mutable bool m_changed = true;
	};
}

#endif // !_VAR_H_
