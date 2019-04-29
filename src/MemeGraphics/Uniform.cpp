#include <MemeGraphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Uniform::Uniform()
		: name(String())
		, type(None)
		, data(NULL)
		, flag(0)
	{
	}
	
	Uniform::Uniform(const String & name)
		: name(name)
		, type(None)
		, data(NULL)
		, flag(0)
	{
	}
	
	Uniform::Uniform(const String & name, const int32_t type)
		: name(name)
		, type(type)
		, data(NULL)
		, flag(0)
	{
	}
	
	Uniform::Uniform(const String & name, const int32_t type, const void * data, int32_t flag)
		: name(name)
		, type(type)
		, data(std::remove_cv_t<void *>(data))
		, flag(flag)
	{
	}

	Uniform::Uniform(const Uniform & copy, const void * data, int32_t flag)
		: name(copy.name)
		, type(copy.type)
		, data(std::remove_cv_t<void *>(data))
		, flag(flag)
	{
	}
	
	Uniform::Uniform(const Uniform & copy)
		: name(copy.name)
		, type(copy.type)
		, data(copy.data)
		, flag(copy.flag)
	{
	}
	
	Uniform::~Uniform()
	{
		if (data)
		{
			switch (flag)
			{
			case 1:
				break;

			case 2:
				//ML_free(data);
				break;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Uniform::serialize(std::ostream & out) const
	{
		out << name << " " << type << " ";
	}

	void Uniform::deserialize(std::istream & in)
	{
		in >> name >> type;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Uniform::equals(const Uniform & other) const
	{
		return (name == other.name) && (type == other.type);
	}

	bool Uniform::lessThan(const Uniform & other) const
	{
		return (name < other.name) && (type < other.type);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}