#ifndef _VAR_H_
#define _VAR_H_

#include <MemeCore/IComparable.h>
#include <MemeScript/TokenList.h>
#include <MemeCore/StringUtility.h>
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
			std::string	name;
			int32_t		index;

			Ptr();
			Ptr(int32_t index, const std::string& name);
			Ptr(const Ptr& copy);

			Var * get() const;

			void serialize(std::ostream & out) const override;
		};

	public:
		enum Type : int
		{
			INVALID_TYPE = -1,

			//- Basic - - -//
			MIN_BASIC,
			Void = MIN_BASIC,
			Bool,
			Float,
			Integer,
			Pointer,
			String,
			MAX_BASIC,

			//- Complex - -//
			MIN_COMPLEX,
			Array = MIN_COMPLEX,
			Func,
			MAX_COMPLEX,

			//- - - - - - -//
			MAX_VAR_TYPE
		};
		
		static const std::string TypeNames[Type::MAX_VAR_TYPE];
		
		inline friend std::ostream & operator<<(std::ostream & out, const Type & rhs)
		{
			return out << Var::TypeNames[rhs];;
		}


	public:
		Var();
		Var(const Type& type, const TokenList& data);
		Var(const Var & other);
		~Var();

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
		bool	isPointerType() const;
		bool	isStringType() const;
		bool	isVoidType() const;

	public: // Data Functions
		bool		boolValue() const;
		float		floatValue() const;
		Var			elemValue(std::size_t i) const;
		std::string	errorValue() const;
		int			intValue() const;
		Ptr			pointerValue() const;
		std::string	stringValue() const;
		std::string	textValue() const;
		TokenList	tokensValue() const;

		Var &		arrayValue(const TokenList & value);
		Var &		boolValue(const bool & value);
		Var &		elemValue(std::size_t index, const Token & value);
		Var &		errorValue(const std::string & value);
		Var &		floatValue(const float & value);
		Var &		funcValue(const TokenList & value);
		Var &		intValue(const int & value);
		Var &		nullValue();
		Var &		pointerValue(const Ptr & value);
		Var &		stringValue(const std::string & value);
		Var &		tokensValue(const TokenList & value);
		Var &		voidValue();
		
		template<typename T, typename ... A>
		inline Var & errorValue(const std::string & fmt, const T & arg0, const A &... args)
		{
			return errorValue(StringUtility::Format(fmt, arg0, (args)...));
		};

	
	public: // Operator Functions
		bool	And(const Var & other) const;
		bool	Equals(const Var & other) const;
		bool	GreaterThan(const Var & other) const;
		bool	LessThan(const Var & other) const;
		bool	Or(const Var & other) const;

		Var &	Add(const Var & other);
		Var &	Div(const Var & other);
		Var &	Mod(const Var & other);
		Var &	Mul(const Var & other);
		Var &	Pow(const Var & other);
		Var &	Set(const Var & other);
		Var &	Sub(const Var & other);

		inline bool equals(const Var & other) const { return Equals(other); }
		inline bool lessThan(const Var & other) const { return LessThan(other); }
		inline bool greaterThan(const Var & other) const { return GreaterThan(other); }

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
		Var & operator=(int value);
		Var & operator=(const Ptr & value);
		Var & operator=(const std::string & value);
		Var & operator=(const char * value);
		Var & operator=(char value);

		inline operator bool() const { return !isErrorType(); }

	public: // Serialization
		void serialize(std::ostream & out) const override;

		static std::ostream& PrintList(std::ostream & out, const Var & value);
		
	public: // Factory
		static Var makeSingle(const Token & tok);
		static Var makeRecursive(const TokenList & toks);

	public: // Helper Functions
		inline int		getScope() const
		{
			return m_scope;
		}
		inline Type		getType() const
		{
			return m_type;
		}

		inline Var &	setScope(int value)
		{
			m_scope = value;
			return (*this);
		}
		inline Var &	setType(const Type & value)
		{
			m_changed = !compareType(value);
			m_type = value;
			return (*this);
		}

	private:
		TokenList	m_tokens;
		int			m_scope;
		Type		m_type;

		mutable bool m_changed = true;
	};
}

#endif // !_VAR_H_
