#include <MemeScript/Property.h>
#include <iomanip>

namespace ml
{
	const std::string Property::TypeNames[Property::MAX_TYPE] = {
		"None",
		"Bool",
		"Char",
		"Double",
		"Float",
		"Int",
		"String",
	};


	Property::Property()
		: m_name(std::string())
		, m_type(Property::None)
		, m_data(std::string())
	{
	}

	Property::Property(const std::string & name)
		: m_name(name)
		, m_type(Property::None)
		, m_data(std::string())
	{
	}

	Property::Property(const std::string & name, Type type, const std::string & data)
		: m_name(name)
		, m_type(type)
		, m_data(data)
	{
	}

	Property::Property(const std::string & name, bool value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, char value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, double value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, float value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, int value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, const std::string & value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const std::string & name, const char * value)
		: m_name(name)
	{
		(*this) = value;
	}

	Property::Property(const Property & copy)
		: m_name(copy.name())
		, m_type(copy.type())
		, m_data(copy.data())
	{
	}

	Property::~Property()
	{
	}


	bool Property::boolValue() const
	{
		if (type() == Property::Bool)
		{
			return (data() == "1" || data() == "true");
		}
		return false;
	}

	char Property::charValue() const
	{
		if (type() == Property::Char)
		{
			return m_data.front();
		}
		return (char)0;
	}

	double Property::doubleValue() const
	{
		if (type() == Property::Double)
		{
			return std::stod(m_data);
		}
		return 0.0;
	}

	float Property::floatValue() const
	{
		if (type() == Property::Float)
		{
			return std::stof(m_data);
		}
		return 0.f;
	}

	int Property::intValue() const
	{
		if (type() == Property::Int)
		{
			return std::stoi(m_data);
		}
		return 0;
	}

	std::string Property::stringValue() const
	{
		if (type() == Property::String)
		{
			return m_data;
		}
		return std::string();
	}


	Property & Property::boolValue(bool value)
	{
		return type(Property::Bool).data(value ? "true" : "false");
	}

	Property & Property::charValue(char value)
	{
		return type(Property::Char).data(std::string(1, value));
	}

	Property & Property::doubleValue(double value)
	{
		return type(Property::Double).data(std::to_string(value));
	}

	Property & Property::floatValue(float value)
	{
		return type(Property::Float).data(std::to_string(value));
	}

	Property & Property::intValue(int value)
	{
		return type(Property::Int).data(std::to_string(value));
	}

	Property & Property::stringValue(const std::string & value)
	{
		return type(Property::String).data(value);
	}


	void Property::serialize(std::ostream & out) const
	{
		out << std::left 
			<< std::setw(10) << type()
			<< std::setw(10) << name();

		switch (type())
		{
		case Property::Bool:
			out << (boolValue() ? "true" : "false");
			break;
		case Property::Char:
			out << "\'" << (charValue()) << "\'";
			break;
		case Property::Double:
			out << (doubleValue());
			break;
		case Property::Float:
			out << (floatValue()) << "f";
			break;
		case Property::Int:
			out << (intValue());
			break;
		case Property::String:
			out << "\"" << (stringValue()) << "\"";
			break;
		case Property::None:
		default:
			out << "";
			break;
		}
	}

	void Property::deserialize(std::istream & in)
	{
		in >> m_name;
		in >> m_type;
		in >> m_data;
	}
	
	bool Property::equals(const Property & value) const
	{
		return
			(type() == value.type()) &&
			(data() == value.data());
	}

	bool Property::lessThan(const Property & value) const
	{
		return
			(type() == value.type()) &&
			(data() < value.data());
	}
	
}
