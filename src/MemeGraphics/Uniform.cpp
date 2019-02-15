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
}