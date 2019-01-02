#include <MemeScript/Var.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/StringUtility.h>
#include <MemeScript/Operator.h>
#include <MemeScript/Interpreter.h>

namespace ml
{
	Var::Ptr::Ptr()
		: index(0)
		, name(std::string())
	{
	}
	
	Var::Ptr::Ptr(int32_t index, const std::string & name)
		: index(index)
		, name(name)
	{
	}
	
	Var::Ptr::Ptr(const Ptr & copy)
		: index(copy.index)
		, name(copy.name)
	{
	}
	
	Var * Var::Ptr::get() const
	{
		if (StringUtility::IsName(name))
		{
			if (Var * v = ML_Interpreter.memory()->getVar(index, name))
			{
				return v;
			}
		}
		return NULL;
	}

	void Var::Ptr::serialize(std::ostream & out) const
	{
		if (Var * var = get())
		{
			out << (*var);
		}
		else
		{
			out << (FG::White | BG::Red) << name;
		}
	}
}

namespace ml
{
	const std::string Var::TypeNames[Type::MAX_VAR_TYPE] =
	{
		"void",
		"bool",
		"float",
		"int",
		"ptr",
		"std::string",
		"array"
		"func",
	};

	Var::Var()
		: m_scope(0)
		, m_type(Type::Void)
		, m_tokens(TokenList())
	{
	}

	Var::Var(const Type & type, const TokenList & data)
		: m_scope(0)
		, m_type(type)
		, m_tokens(data)
	{
	}

	Var::Var(const Var & copy)
		: m_scope(copy.m_scope)
		, m_type(copy.m_type)
		, m_tokens(copy.m_tokens)
	{
	}

	Var::~Var()
	{
	}


	// Check Functions

	bool Var::compareTokens(const TokenList& value) const
	{
		return tokensValue() == value;
	}

	bool Var::compareType(const Type& type) const
	{
		return getType() == type;
	}


	bool Var::isValid() const
	{
		if (m_changed)
		{
			m_changed = false;

			switch (getType())
			{
			case Var::Func:
				return !(textValue()).empty();

			case Var::Bool:
				return isBoolType();

			case Var::Float:
				return isFloatType();

			case Var::Integer:
				return isIntType();

			case Var::Pointer:
				return isPointerType();

			case Var::String:
			default:
				return true;
			}
		}
		return true;
	}
		 
	bool Var::isArrayType() const
	{
		return compareType(Var::Array);
	}
		 
	bool Var::isBasicType() const
	{
		return getType() >= Var::MIN_BASIC && getType() < Var::MAX_BASIC;
	}
		 
	bool Var::isBoolType() const
	{
		return isNameType() && StringUtility::IsBool((textValue()));
	}
		 
	bool Var::isComplexType() const
	{
		return getType() >= Var::MIN_COMPLEX && getType() < Var::MAX_COMPLEX;
	}
		 
	bool Var::isEmptyValue() const
	{
		return m_tokens.empty();
	}
		 
	bool Var::isErrorType() const
	{
		return tokensValue().front(TokenType::TOK_ERR);
	}
		 
	bool Var::isFloatType() const
	{
		return tokensValue().front() == 'f' && StringUtility::IsDecimal((textValue()));
	}
		 
	bool Var::isFuncType() const
	{
		return compareType(Var::Func) && isValid();
	}
		 
	bool Var::isIntType() const
	{
		return tokensValue().front() == 'i' && StringUtility::IsInt((textValue()));
	}
		 
	bool Var::isNameType() const
	{
		return m_tokens.front() == 'n' && StringUtility::IsName((textValue()));
	}
		 
	bool Var::isNullValue() const
	{
		return isVoidType() || tokensValue().empty();
	}
		 
	bool Var::isPointerType() const
	{
		return compareType(Var::Pointer) && isNameType();
	}
		 
	bool Var::isStringType() const
	{
		return compareType(Var::String) || tokensValue().front('s');
	}
		 
	bool Var::isVoidType() const
	{
		return compareType(Var::Void) || tokensValue().front(TokenType::TOK_VOID);
	}


	// Data Functions

	bool		Var::boolValue() const
	{
		return isValid() ? StringUtility::ToBool((textValue())) : false;
	}

	float		Var::floatValue() const
	{
		return isValid() ? StringUtility::ToFloat((textValue())) : 0;
	}

	Var			Var::elemValue(std::size_t i) const
	{
		if (!isEmptyValue())
		{
			if (isArrayType())
			{
				if (i < m_tokens.size())
				{
					const Token& t = m_tokens.at(i);

					return Var::makeSingle(t);
				}
				else
				{
					return Var().errorValue("Index out of range {0}");
				}
			}
			else if (isStringType())
			{
				const std::string str = stringValue();
				if (i < str.size())
				{
					return Var().stringValue(std::string(1, str[i]));
				}
			}
		}
		return Var().errorValue("Var : Cannot access element {0}[{1}]", *this, i);
	}

	std::string	Var::errorValue() const
	{
		return isErrorType() ? (textValue()) : std::string();
	}

	int			Var::intValue() const
	{
		return isValid() ? StringUtility::ToInt((textValue())) : 0;
	}

	Var::Ptr	Var::pointerValue() const
	{
		return Ptr(m_scope, (textValue()));
	}
	
	std::string	Var::stringValue() const
	{
		return isValid() ? (textValue()) : std::string();
	}

	std::string	Var::textValue() const
	{
		return m_tokens.str();
	}

	TokenList	Var::tokensValue() const
	{
		if (compareType(Var::Func))
		{
			return m_tokens;
		}
		else if (isPointerType())
		{
			if (Var * var = pointerValue().get())
			{
				return var->tokensValue();
			}
		}
		return m_tokens;
	}

	
	Var &	Var::arrayValue(const TokenList & value)
	{
		return setType(Var::Array).tokensValue(value);
	}

	Var &	Var::boolValue(const bool & value)
	{
		return setType(Var::Bool).tokensValue({ { TokenType::TOK_NAME, (value ? "true" : "false") } });
	}

	Var &	Var::elemValue(std::size_t index, const Token & value)
	{
		if (tokensValue().inRange(index))
		{
			m_changed = true;
			m_tokens[index] = value;
		}
		return (*this);
	}

	Var &	Var::errorValue(const std::string & value)
	{
		return voidValue().tokensValue({ { TokenType::TOK_ERR, value } });
	}

	Var &	Var::floatValue(const float & value)
	{
		return setType(Var::Float).tokensValue({ { TokenType::TOK_FLT, std::to_string(value) } });
	}

	Var &	Var::funcValue(const TokenList & value)
	{
		return setType(Var::Func).tokensValue(value);
	}

	Var &	Var::intValue(const int & value)
	{
		return setType(Var::Integer).tokensValue({ { TokenType::TOK_INT, std::to_string(value) } });
	}
	
	Var &	Var::nullValue()
	{
		return voidValue().tokensValue({});
	}

	Var &	Var::pointerValue(const Ptr & value)
	{
		return setType(Var::Pointer).tokensValue({ { TokenType::TOK_NAME, value.name } });
	}

	Var &	Var::stringValue(const std::string & value)
	{
		return setType(Var::String).tokensValue({ { TokenType::TOK_STR, value } });
	}

	Var &	Var::tokensValue(const TokenList & value)
	{
		m_changed = !compareTokens(value);
		m_tokens = value;
		return (*this);
	}

	Var &	Var::voidValue()
	{
		return setType(Var::Void).tokensValue({ TokenType::TOK_VOID });
	}


	// Operator Functions

	bool	Var::And(const Var & other) const
	{
		bool lp = isPointerType() && isValid();
		bool rp = other.compareType(Var::Pointer) && other.isValid();
		if (lp && rp)
		{
			return pointerValue().get()->And(*other.pointerValue().get());
		}
		else if (!lp && rp)
		{
			return pointerValue().get()->And(*other.pointerValue().get());
		}
		else if (lp && !rp)
		{
			return pointerValue().get()->And(other);
		}

		switch (getType())
		{
			// Bool
		case Type::Bool:
			switch (other.getType())
			{
			case Type::Integer:
				return boolValue() && other.intValue();

			case Type::Bool:
				return boolValue() && other.boolValue();
			}

			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue() && other.intValue();

			case Type::Bool:
				return intValue() && other.boolValue();
			}
		}

		Debug::LogError("Invalid Operation: \'{0}\' {1} \'{2}\'",
			(*this), OperatorType::OP_AND, other);
		return false;
	}

	bool	Var::Equals(const Var & other) const
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue() == other.intValue();

			case Type::Float:
				return intValue() == other.floatValue();
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Integer:
				return floatValue() == other.intValue();

			case Type::Float:
				return floatValue() == other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue().get()->Equals(*other.pointerValue().get());
			}
			else if (!lp && rp)
			{
				return Equals(*other.pointerValue().get());
			}
			else if (lp && !rp)
			{
				return pointerValue().get()->Equals(other);
			}
		}

		Debug::LogError("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_EQU, other.getType(), other);

		return false;
	}

	bool	Var::GreaterThan(const Var & other) const
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue() > other.intValue();

			case Type::Float:
				return intValue() > other.floatValue();
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Integer:
				return floatValue() > other.intValue();

			case Type::Float:
				return floatValue() > other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue().get()->GreaterThan(*other.pointerValue().get());
			}
			else if (!lp && rp)
			{
				return GreaterThan(*other.pointerValue().get());
			}
			else if (lp && !rp)
			{
				return pointerValue().get()->GreaterThan(other);
			}
		}

		Debug::LogError("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_GT, other.getType(), other);

		return false;
	}

	bool	Var::LessThan(const Var & other) const
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue() < other.intValue();

			case Type::Float:
				return intValue() < other.floatValue();
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Integer:
				return floatValue() < other.intValue();

			case Type::Float:
				return floatValue() < other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue().get()->LessThan(*other.pointerValue().get());
			}
			else if (!lp && rp)
			{
				return LessThan(*other.pointerValue().get());
			}
			else if (lp && !rp)
			{
				return pointerValue().get()->LessThan(other);
			}
		}

		Debug::LogError("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_LT, other.getType(), other);

		return false;
	}

	bool	Var::Or(const Var & other) const
	{
		bool lp = compareType(Var::Pointer) && isValid();
		bool rp = other.compareType(Var::Pointer) && other.isValid();
		if (lp && rp)
		{
			return pointerValue().get()->Or(*other.pointerValue().get());
		}
		else if (!lp && rp)
		{
			return pointerValue().get()->Or(*other.pointerValue().get());
		}
		else if (lp && !rp)
		{
			return pointerValue().get()->Or(other);

		}
		switch (getType())
		{
			// Bool
		case Type::Bool:
			switch (other.getType())
			{
			case Type::Integer:
				return boolValue() || other.intValue();

			case Type::Bool:
				return boolValue() || other.boolValue();
			}

			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue() || other.intValue();

			case Type::Bool:
				return intValue() || other.boolValue();
			}
		}

		Debug::LogError("Invalid Operation: \'{0}\' {1} \'{2}\'",
			(*this), OperatorType::OP_OR, other);
		return false;
	}


	Var &	Var::Add(const Var & other)
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue(intValue() + other.intValue());

			case Type::Float:
				return intValue(intValue() + (int)other.floatValue());

			default:
				if (other.isIntType())
					return intValue(intValue() + other.intValue());
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Float:
				return floatValue(floatValue() + other.floatValue());

			case Type::Integer:
				return floatValue(floatValue() + (float)other.intValue());

			default:
				if (other.isFloatType())
					return floatValue(floatValue() + other.floatValue());
			}

			// String
		case Type::String:
			switch (other.getType())
			{
			case Var::String:
			default:
				return stringValue(stringValue() + other.stringValue());
			}
		}

		bool lp = compareType(Var::Pointer);
		bool rp = other.compareType(Var::Pointer);
		if (lp || rp)
		{
			bool lg = isPointerType();
			bool rg = other.isPointerType();
			if (lg && rg)
			{
				return pointerValue().get()->Add(*other.pointerValue().get());
			}
			else if (!lg && rg)
			{
				return Add(*other.pointerValue().get());
			}
			else if (lg && !rg)
			{
				return Set(*pointerValue().get()).Add(other);
			}
		}

		//return (*this);

		return errorValue(StringUtility::Format("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_ADD, other.getType(), other));
	}

	Var &	Var::Div(const Var & other)
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue(intValue() / other.intValue());

			case Type::Float:
				return intValue(intValue() / (int)other.floatValue());

			default:
				if (other.isIntType())
					return intValue(intValue() / other.intValue());
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Float:
				return floatValue(floatValue() / other.floatValue());

			case Type::Integer:
				return floatValue(floatValue() / (float)other.intValue());

			default:
				if (other.isFloatType())
					return floatValue(floatValue() / other.floatValue());
			}

			//	// String
			//case TokenType::String:
			//	switch (other.getType())
			//	{
			//	case Var::String:
			//	default:
			//		return stringValue(stringValue() / other.tokensValue());
			//	}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue().get()->Div(*other.pointerValue().get());
				}
				else if (!lp && rp)
				{
					return Div(*other.pointerValue().get());
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Div(other);
				}
			}
		}

		return errorValue(StringUtility::Format("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_DIV, other.getType(), other));
	}

	Var &	Var::Mul(const Var & other)
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue(intValue() * other.intValue());

			case Type::Float:
				return intValue(intValue() * (int)other.floatValue());

			default:
				if (other.isIntType())
					return intValue(intValue() * other.intValue());
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Float:
				return floatValue(floatValue() * other.floatValue());

			case Type::Integer:
				return floatValue(floatValue() * (float)other.intValue());

			default:
				if (other.isFloatType())
					return floatValue(floatValue() * other.floatValue());
			}

			//	// String
			//case TokenType::String:
			//	switch (other.getType())
			//	{
			//	case Var::String:
			//	default:
			//		return stringValue(stringValue() * other.tokensValue());
			//	}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue().get()->Mul(*other.pointerValue().get());
				}
				else if (!lp && rp)
				{
					return Mul(*other.pointerValue().get());
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Mul(other);
				}
			}
		}

		return errorValue(StringUtility::Format("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_MUL, other.getType(), other));
	}

	Var &	Var::Pow(const Var & other)
	{
		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue().get()->Pow(*other.pointerValue().get());
				}
				else if (!lp && rp)
				{
					return Pow(*other.pointerValue().get());
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Pow(other);
				}
			}
		}

		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue((int)pow(intValue(), other.intValue()));

			case Type::Float:
				return intValue((int)pow(intValue(), (int)other.floatValue()));

			default:
				if (other.isIntType())
					return intValue((int)pow(intValue(), other.intValue()));
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Float:
				return floatValue(powf(floatValue(), other.floatValue()));

			case Type::Integer:
				return floatValue(powf(floatValue(), (float)other.intValue()));

			default:
				if (other.isFloatType())
					return floatValue(powf(floatValue(), other.floatValue()));
			}
		}

		return errorValue(StringUtility::Format("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_POW, other.getType(), other));
	}

	Var &	Var::Set(const Var & other)
	{
		if (Var * v = other.pointerValue().get())
		{
			return Set(*v);
		}
		return setType(other.getType()).tokensValue(other.tokensValue());
	}

	Var &	Var::Sub(const Var & other)
	{
		switch (getType())
		{
			// Int
		case Type::Integer:
			switch (other.getType())
			{
			case Type::Integer:
				return intValue(intValue() - other.intValue());

			case Type::Float:
				return intValue(intValue() - (int)other.floatValue());

			default:
				if (other.isIntType())
					return intValue(intValue() - other.intValue());
			}

			// Float
		case Type::Float:
			switch (other.getType())
			{
			case Type::Float:
				return floatValue(floatValue() - other.floatValue());

			case Type::Integer:
				return floatValue(floatValue() - (float)other.intValue());

			default:
				if (other.isFloatType())
					return floatValue(floatValue() - other.floatValue());
			}

			//	// String
			//case TokenType::String:
			//	switch (other.getType())
			//	{
			//	case Var::String:
			//	default:
			//		return stringValue(stringValue() - other.tokensValue());
			//	}
		}

		bool lp = compareType(Var::Pointer);
		bool lg = isValid();
		bool rp = other.compareType(Var::Pointer);
		bool rg = other.isValid();
		if (lp || rp)
		{
			if (lg && rg)
			{
				if (lp && rp)
				{
					return pointerValue().get()->Sub(*other.pointerValue().get());
				}
				else if (!lp && rp)
				{
					return Sub(*other.pointerValue().get());
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Sub(other);
				}
			}
		}

		return errorValue(StringUtility::Format("Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getType(), (*this), OperatorType::OP_SUB, other.getType(), other));
	}


	// Operators

	bool operator&&(const Var & lhs, const Var & rhs)
	{
		return lhs.And(rhs);
	}

	bool operator||(const Var & lhs, const Var & rhs)
	{
		return lhs.Or(rhs);
	}


	Var & operator+=(Var & lhs, const Var & rhs)
	{
		return lhs.Add(rhs);
	}

	Var & operator-=(Var & lhs, const Var & rhs)
	{
		return lhs.Sub(rhs);
	}

	Var & operator*=(Var & lhs, const Var & rhs)
	{
		return lhs.Mul(rhs);
	}

	Var & operator/=(Var & lhs, const Var & rhs)
	{
		return lhs.Div(rhs);
	}

	Var & operator^=(Var & lhs, const Var & rhs)
	{
		return lhs.Pow(rhs);
	}


	Var operator+(const Var & lhs, const Var & rhs)
	{
		return Var().Set(lhs).Add(rhs);
	}

	Var operator-(const Var & lhs, const Var & rhs)
	{
		return Var().Set(lhs).Sub(rhs);
	}

	Var operator*(const Var & lhs, const Var & rhs)
	{
		return Var().Set(lhs).Mul(rhs);
	}

	Var operator/(const Var & lhs, const Var & rhs)
	{
		return Var().Set(lhs).Div(rhs);
	}

	Var operator^(const Var & lhs, const Var & rhs)
	{
		return Var().Set(lhs).Pow(rhs);
	}


	Var & Var::operator=(const Var & other)
	{
		return Set(other);
	}

	Var & Var::operator=(bool value)
	{
		return boolValue(value);
	}

	Var & Var::operator=(float value)
	{
		return floatValue(value);
	}

	Var & Var::operator=(double value)
	{
		return floatValue((float)value);
	}

	Var & Var::operator=(int value)
	{
		return intValue(value);
	}

	Var & Var::operator=(const Ptr& value)
	{
		return pointerValue(value);
	}

	Var & Var::operator=(const std::string& value)
	{
		return stringValue(value);
	}

	Var & Var::operator=(const char * value)
	{
		return stringValue(value);
	}

	Var & Var::operator=(char value)
	{
		return stringValue(std::string(1, value));
	}
	

	// Serialization

	void Var::serialize(std::ostream & out) const
	{
		out << FMT();

		if (!isValid())
		{
			out << (FG::Black | BG::Red) << textValue() << FMT();
			return;
		}

		switch (getType())
		{
		case Var::Bool:
			out << (FG::Cyan | BG::Black)
				<< (boolValue() ? "true" : "false")
				<< FMT();
			break;

		case Var::Float:
			out << (FG::Yellow | BG::Black)
				<< floatValue() << "f"
				<< FMT();
			break;

		case Var::Integer:
			out << (FG::Yellow | BG::Black)
				<< intValue()
				<< FMT();
			break;

		case Var::Pointer:
			out << (FG::Gray | BG::Black)
				<< "" << pointerValue()
				<< FMT();
			break;

		case Var::String:
			out << (FG::Magenta | BG::Black)
				<< '\"' << stringValue() << '\"'
				<< FMT();
			break;

		case Var::Func:
			out << (FG::White | BG::DarkGray) << "[](" << FMT();
			Var::PrintList(out, (*this));
			out << (FG::White | BG::DarkGray) << ")" << FMT();
			break;

		case Var::Array:
			out << (FG::Black | BG::Yellow) << "[" << FMT() << " ";
			Var::PrintList(out, (*this));
			out << " " << (FG::Black | BG::Yellow) << "]" << FMT();
			break;

		case Var::Void:
			out << (FG::Black | BG::White) << textValue();
			break;
		}
	}
	
	std::ostream & Var::PrintList(std::ostream & out, const Var & value)
	{
		out << FMT();
		const TokenList& data = value.tokensValue();
		for (TokenList::const_iterator it = data.cbegin(); it != data.cend(); it++)
		{
			out << Var::makeSingle(*it) << (it != data.cend() - 1 ? ", " : "") << FMT();
		}
		return out;
	}


	// Factory

	Var Var::makeSingle(const Token & tok)
	{
		switch (tok.type)
		{
		case 's': 
			return Var().stringValue(tok.data);
		case 'i': 
			return Var().intValue(std::stoi(tok.data));
		case 'f': 
			return Var().floatValue(std::stof(tok.data));
		case 'n':
			return StringUtility::IsBool(tok.data)
				? Var().boolValue(StringUtility::ToBool(tok.data))
				: Var().pointerValue(Var::Ptr(0, tok.data));
		default: 
			return Var().errorValue(tok.str());
		}
	}

	Var Var::makeRecursive(const TokenList & toks)
	{
		switch (toks.size())
		{
		case 0:	
			return Var().nullValue();
		case 1:	
			return Var::makeSingle(toks.front());
		default:
			return Var().arrayValue(toks);
		}
	}

}