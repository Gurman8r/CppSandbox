#include <MemeScript/Var.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeScript/StringUtility.hpp>
#include <MemeScript/Operator.hpp>
#include <MemeScript/Interpreter.hpp>
#include <MemeCore/BitHelper.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Var::Ptr::Ptr()
		: index(0)
		, name(String())
	{
	}
	
	Var::Ptr::Ptr(int32_t index, const String & name)
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
			if (Var * v = ML_Runtime.getVar(index, name))
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

	/* * * * * * * * * * * * * * * * * * * * */
}

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const String Var::TypeNames[Var::MAX_VAR_TYPE] =
	{
		"void",
		"bool",
		"float",
		"int",
		"pointer",
		"string",
		"array"
		"func",
		"struct",
	};

	/* * * * * * * * * * * * * * * * * * * * */

	Var::Var()
		: m_scope(0)
		, m_typeID(Var::Void)
		, m_data(TokenList())
	{
	}

	Var::Var(const int32_t & type, const TokenList & data)
		: m_scope(0)
		, m_typeID(type)
		, m_data(data)
	{
	}

	Var::Var(const Var & copy)
		: m_scope(copy.m_scope)
		, m_typeID(copy.m_typeID)
		, m_data(copy.m_data)
	{
	}

	Var::~Var()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Var::compareTokens(const TokenList& value) const
	{
		return tokensValue() == value;
	}

	bool Var::compareType(const int32_t& type) const
	{
		return getTypeID() == type;
	}


	bool Var::isValid() const
	{
		if (m_changed)
		{	m_changed = false;

			switch (getTypeID())
			{
			case Var::Func:		return !isEmptyValue();
			case Var::Bool:		return isBoolType();
			case Var::Float:	return isFloatType();
			case Var::Integer:	return isIntType();
			case Var::Pointer:	return isPointerType();
			
			case Var::Str:
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
		return getTypeID() >= Var::MIN_BASIC && getTypeID() < Var::MAX_BASIC;
	}
		 
	bool Var::isBoolType() const
	{
		return isNameType() && StringUtility::IsBool(textValue());
	}
		 
	bool Var::isComplexType() const
	{
		return getTypeID() >= Var::MIN_COMPLEX && getTypeID() < Var::MAX_COMPLEX;
	}
		 
	bool Var::isEmptyValue() const
	{
		return m_data.empty();
	}
		 
	bool Var::isErrorType() const
	{
		return tokensValue().front('\0');
	}
		 
	bool Var::isFloatType() const
	{
		return tokensValue().front('f') && StringUtility::IsDecimal(textValue());
	}
		 
	bool Var::isFuncType() const
	{
		return compareType(Var::Func) && isValid();
	}
		 
	bool Var::isIntType() const
	{
		return tokensValue().front('i') && StringUtility::IsInt(textValue());
	}
		 
	bool Var::isNameType() const
	{
		return m_data.front('n') && StringUtility::IsName(textValue());
	}
		 
	bool Var::isNullValue() const
	{
		return isVoidType() || tokensValue().empty();
	}

	bool Var::isNumericType() const
	{
		return isBoolType() || isIntType() || isFloatType();
	}
		 
	bool Var::isPointerType() const
	{
		return compareType(Var::Pointer) && isNameType();
	}
		 
	bool Var::isStringType() const
	{
		return compareType(Var::Str) || tokensValue().front('s');
	}

	bool Var::isStructType() const
	{
		return compareType(Var::Struct);
	}

	bool Var::isVoidType() const
	{
		return compareType(Var::Void) || tokensValue().front(Token::T_Void);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool		Var::boolValue() const
	{
		if (isBoolType())
		{
			return StringUtility::ToBool(textValue());
		}
		else if (isIntType())
		{
			return intValue();
		}
		else if (isFloatType())
		{
			return floatValue();
		}
		else if (isStringType())
		{
			return !stringValue().empty();
		}
		else if (isNameType())
		{
			return pointerValue()->boolValue();
		}
		return (!isEmptyValue() && !isErrorType() && !isVoidType());
	}

	TokenList	Var::dataValue() const
	{
		return m_data;
	}

	float		Var::floatValue() const
	{
		return isValid() ? StringUtility::ToFloat(textValue()) : 0;
	}

	Var			Var::elemValue(size_t i) const
	{
		if (!isEmptyValue())
		{
			if (isArrayType())
			{
				if (i < dataValue().size())
				{
					const Token& t = dataValue().at(i);

					return Var::makeSingle(t);
				}
				else
				{
					return Var().errorValue("Index out of range {0}");
				}
			}
			else if (isStringType())
			{
				const String str = stringValue();
				if (i < str.size())
				{
					return Var().stringValue(String(1, str[i]));
				}
			}
			else if (isIntType())
			{
				return Var().boolValue(bitRead(intValue(), i));
			}
		}
		return Var().errorValue("Var : Cannot access element {0}[{1}]", *this, i);
	}

	String		Var::errorValue() const
	{
		return isErrorType() ? (textValue()) : String();
	}

	int32_t		Var::intValue() const
	{
		return isValid() ? StringUtility::ToInt(textValue()) : 0;
	}

	Var::Ptr	Var::pointerValue() const
	{
		return Ptr(m_scope, (textValue()));
	}

	int32_t		Var::sizeOfValue() const
	{
		if (isArrayType())
		{
			return (int32_t)tokensValue().size();
		}
		else if (isStringType())
		{
			return (int32_t)stringValue().size();
		}
		else if (isNumericType())
		{
			return intValue();
		}
		return 0;
	}
	
	String		Var::stringValue() const
	{
		return isValid() ? (textValue()) : String();
	}

	String		Var::textValue() const
	{
		return dataValue().str();
	}

	TokenList	Var::tokensValue() const
	{
		if (compareType(Var::Func))
		{
			return dataValue();
		}
		else if (isPointerType())
		{
			if (Var * var = (*pointerValue()))
			{
				return var->tokensValue();
			}
		}
		return dataValue();
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	Var & Var::arrayValue(const TokenList & value)
	{
		return setType(Var::Array).dataValue(value);
	}
		  
	Var & Var::boolValue(const bool & value)
	{
		return setType(Var::Bool).dataValue(Token('n', (value ? "true" : "false")));
	}

	Var & Var::dataValue(const TokenList & value)
	{
		m_changed = !compareTokens(value);
		m_data = value;
		return (*this);
	}

	Var & Var::elemValue(size_t index, const Token & value)
	{
		if (isArrayType() || isStringType())
		{
			if (tokensValue().inRange(index))
			{
				m_changed = true;
				m_data[index] = value;
			}
		}
		return (*this);
	}
		  
	Var & Var::errorValue(const String & value)
	{
		return voidValue().dataValue(Token('\0', value));
	}
		  
	Var & Var::floatValue(const float & value)
	{
		return setType(Var::Float).dataValue(Token('f', std::to_string(value)));
	}
		  
	Var & Var::funcValue(const TokenList & value)
	{
		return setType(Var::Func).dataValue(value);
	}
		  
	Var & Var::intValue(const int32_t & value)
	{
		return setType(Var::Integer).dataValue(Token('i', std::to_string(value)));
	}
 		  
	Var & Var::nullValue()
	{
		return voidValue().dataValue({});
	}
		  
	Var & Var::pointerValue(const Ptr & value)
	{
		return setType(Var::Pointer).dataValue(Token('n', value.name));
	}
		  
	Var & Var::stringValue(const String & value)
	{
		return setType(Var::Str).dataValue(Token('s', value));
	}

	Var & Var::structValue(const TokenList & value)
	{
		return setType(Var::Struct).dataValue(value);
	}
	  
	Var & Var::voidValue()
	{
		return setType(Var::Void).dataValue(Token(' '));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Var & Var::print()
	{
		cout << (*this);
		return (*this);
	}

	void Var::serialize(std::ostream & out) const
	{
		out << FMT();

		if (!isValid())
		{
			out << (FG::Black | BG::Red) << textValue() << FMT();
			return;
		}

		switch (getTypeID())
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
				<< pointerValue()
				<< FMT();
			break;

		case Var::Str:
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

		case Var::Struct:
			out << (FG::White | BG::DarkGray) << "$(" << FMT();
			Var::PrintList(out, (*this));
			out << (FG::White | BG::DarkGray) << ")" << FMT();
			break;

		case Var::Void:
			out << (FG::Black | BG::White) << textValue();
			break;
		}
	}

	std::ostream & Var::PrintList(std::ostream & out, const Var & value)
	{
		out << FMT();
		const TokenList & data = value.tokensValue();
		for (TokenList::const_iterator it = data.cbegin(); it != data.cend(); it++)
		{
			out << Var::makeSingle(*it) << (it != data.cend() - 1 ? ", " : "") << FMT();
		}
		return out;
	}

	/* * * * * * * * * * * * * * * * * * * * */

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
			return Var().errorValue(tok.to_str());
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

	/* * * * * * * * * * * * * * * * * * * * */

	bool Var::And(const Var & other) const
	{
		return boolValue() && other.boolValue();
	}
		 
	bool Var::Or(const Var & other) const
	{
		return boolValue() && other.boolValue();
	}
		 
	bool Var::Equals(const Var & other) const
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue() == other.intValue();
			case Var::Float: return intValue() == other.floatValue();
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Integer: return floatValue() == other.intValue();
			case Var::Float: return floatValue() == other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue()->Equals(*(*other.pointerValue()));
			}
			else if (!lp && rp)
			{
				return Equals(*(*other.pointerValue()));
			}
			else if (lp && !rp)
			{
				return pointerValue()->Equals(other);
			}
		}

		return textValue() == other.textValue();
	}
		 
	bool Var::Less(const Var & other) const
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue() < other.intValue();
			case Var::Float: return intValue() < other.floatValue();
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Integer: return floatValue() < other.intValue();
			case Var::Float: return floatValue() < other.floatValue();
			}
		}

		bool lp = isPointerType();
		bool rp = other.isPointerType();
		if (lp || rp)
		{
			if (lp && rp)
			{
				return pointerValue()->Less(*(*other.pointerValue()));
			}
			else if (!lp && rp)
			{
				return Less(*(*other.pointerValue()));
			}
			else if (lp && !rp)
			{
				return pointerValue()->Less(other);
			}
		}

		return textValue() < other.textValue();
	}


	Var & Var::Add(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() + other.intValue());
			case Var::Float: return intValue(intValue() + (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() + other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() + other.floatValue());
			case Var::Integer: return floatValue(floatValue() + (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() + other.floatValue());
			}

			// String
		case Var::Str:
			switch (other.getTypeID())
			{
			case Var::Str:
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
				return pointerValue()->Add(*(*other.pointerValue()));
			}
			else if (!lg && rg)
			{
				return Add(*(*other.pointerValue()));
			}
			else if (lg && !rg)
			{
				return Set(*pointerValue().get()).Add(other);
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "+", other.getTypeID(), other));
	}
		  
	Var & Var::Div(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() / other.intValue());
			case Var::Float: return intValue(intValue() / (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() / other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() / other.floatValue());
			case Var::Integer: return floatValue(floatValue() / (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() / other.floatValue());
			}
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
					return pointerValue()->Div(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Div(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Div(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "/", other.getTypeID(), other));
	}
		  
	Var & Var::Mod(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() % other.intValue());
			case Var::Float: return intValue(intValue() % (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() % other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return intValue((int32_t)floatValue() % (int32_t)other.floatValue());
			case Var::Integer: return intValue((int32_t)floatValue() % other.intValue());
			default:
				if (other.isFloatType())
					return intValue((int32_t)floatValue() % (int32_t)other.floatValue());
			}
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
					return pointerValue()->Mod(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Mod(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Mod(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "%", other.getTypeID(), other));
	}
		  
	Var & Var::Mul(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() * other.intValue());
			case Var::Float: return intValue(intValue() * (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() * other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() * other.floatValue());
			case Var::Integer: return floatValue(floatValue() * (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() * other.floatValue());
			}
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
					return pointerValue()->Mul(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Mul(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Mul(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "*", other.getTypeID(), other));
	}
		  
	Var & Var::Pow(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue((int32_t)pow(intValue(), other.intValue()));
			case Var::Float: return intValue((int32_t)pow(intValue(), (int32_t)other.floatValue()));
			default:
				if (other.isIntType())
					return intValue((int32_t)pow(intValue(), other.intValue()));
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(powf(floatValue(), other.floatValue()));
			case Var::Integer: return floatValue(powf(floatValue(), (float)other.intValue()));
			default:
				if (other.isFloatType())
					return floatValue(powf(floatValue(), other.floatValue()));
			}
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
					return pointerValue()->Pow(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Pow(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Pow(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "^", other.getTypeID(), other));
	}
		  
	Var & Var::Sub(const Var & other)
	{
		switch (getTypeID())
		{
			// Int
		case Var::Integer:
			switch (other.getTypeID())
			{
			case Var::Integer: return intValue(intValue() - other.intValue());
			case Var::Float: return intValue(intValue() - (int32_t)other.floatValue());
			default:
				if (other.isIntType())
					return intValue(intValue() - other.intValue());
			}

			// Float
		case Var::Float:
			switch (other.getTypeID())
			{
			case Var::Float: return floatValue(floatValue() - other.floatValue());
			case Var::Integer: return floatValue(floatValue() - (float)other.intValue());
			default:
				if (other.isFloatType())
					return floatValue(floatValue() - other.floatValue());
			}
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
					return pointerValue()->Sub(*(*other.pointerValue()));
				}
				else if (!lp && rp)
				{
					return Sub(*(*other.pointerValue()));
				}
				else if (lp && !rp)
				{
					return Set(*pointerValue().get()).Sub(other);
				}
			}
		}

		return errorValue(String::Format(
			"Invalid Operation: {0} \'{1}\' {2} {3} \'{4}\'",
			getTypeID(), (*this), "-", other.getTypeID(), other));
	}
		  
	Var & Var::Set(const Var & other)
	{
		if (Var * v = other.pointerValue().get())
		{
			return Set(*v);
		}
		return setType(other.getTypeID()).dataValue(other.tokensValue());
	}


	/* * * * * * * * * * * * * * * * * * * * */

	bool operator&&(const Var & lhs, const Var & rhs)	{ return lhs.And(rhs); }
	bool operator||(const Var & lhs, const Var & rhs)	{ return lhs.Or(rhs); }

	Var & operator+=(Var & lhs, const Var & rhs)		{ return lhs.Add(rhs); }
	Var & operator-=(Var & lhs, const Var & rhs)		{ return lhs.Sub(rhs); }
	Var & operator*=(Var & lhs, const Var & rhs)		{ return lhs.Mul(rhs); }
	Var & operator/=(Var & lhs, const Var & rhs)		{ return lhs.Div(rhs); }
	Var & operator^=(Var & lhs, const Var & rhs)		{ return lhs.Pow(rhs); }
	Var & operator%=(Var & lhs, const Var & rhs)		{ return lhs.Mod(rhs); }

	Var operator+(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Add(rhs); }
	Var operator-(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Sub(rhs); }
	Var operator*(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Mul(rhs); }
	Var operator/(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Div(rhs); }
	Var operator^(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Pow(rhs); }
	Var operator%(const Var & lhs, const Var & rhs)		{ return Var().Set(lhs).Mod(rhs); }

	Var & Var::operator=(const Var & other)		{ return Set(other); }
	Var & Var::operator=(bool value)			{ return boolValue(value); }
	Var & Var::operator=(float value)			{ return floatValue(value); }
	Var & Var::operator=(double value)			{ return floatValue((float)value); }
	Var & Var::operator=(int32_t value)			{ return intValue(value); }
	Var & Var::operator=(const Ptr & value)		{ return pointerValue(value); }
	Var & Var::operator=(const String & value)	{ return stringValue(value); }
	Var & Var::operator=(CString value)			{ return stringValue(value); }
	Var & Var::operator=(char value)			{ return stringValue(String(1, value)); }
	
	/* * * * * * * * * * * * * * * * * * * * */
}