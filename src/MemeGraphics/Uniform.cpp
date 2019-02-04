#include <MemeGraphics/Uniform.hpp>

namespace ml
{
	const String Uniform::Names[Uniform::MAX_UNIFORM_ID] =
	{
		"u_model",
		"u_view",
		"u_proj",
		"u_color",
		"u_texture",
		"u_curveMode",
	};

	Uniform::Uniform()
		: name(String())
	{
	}
	Uniform::Uniform(const ID id)
		: name(Uniform::Names[id])
	{
	}
	Uniform::Uniform(const String & name)
		: name(name)
	{
	}
	Uniform::Uniform(const char * name)
		: name(name)
	{
	}
	Uniform::Uniform(const Uniform & copy)
		: name(copy.name)
	{
	}
	Uniform::~Uniform()
	{
	}
}