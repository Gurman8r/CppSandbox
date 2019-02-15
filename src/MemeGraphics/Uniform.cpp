#include <MemeGraphics/Uniform.hpp>

namespace ml
{
	Uniform::Uniform()
		: name(String())
		, type(None)
		, data(NULL)
	{
	}
	
	Uniform::Uniform(const String & name)
		: name(name)
		, type(None)
		, data(NULL)
	{
	}
	
	Uniform::Uniform(const String & name, const int32_t type)
		: name(name)
		, type(type)
		, data(NULL)
	{
	}
	
	Uniform::Uniform(const String & name, const int32_t type, const void * data)
		: name(name)
		, type(type)
		, data(data)
	{
	}
	
	Uniform::Uniform(const Uniform & copy)
		: name(copy.name)
		, type(copy.type)
		, data(copy.data)
	{
	}
	
	Uniform::~Uniform()
	{
	}

	bool Uniform::good() const
	{
		return !name.empty() && data && type;
	}

	void Uniform::serialize(std::ostream & out) const
	{
		out << name << " " << type << " ";
	}

	void Uniform::deserialize(std::istream & in)
	{
		in >> name >> type;
	}

	bool Uniform::equals(const Uniform & other) const
	{
		return (name == other.name) && (type == other.type);
	}

	bool Uniform::lessThan(const Uniform & other) const
	{
		return (name < other.name) && (type < other.type);
	}
}