#include <MemeGraphics/Uniform.h>

namespace ml
{
	const std::string Uniform::UniformNames[Uniform::ID::MAX_UNIFORM_ID] =
	{
		"u_color",
		"u_model",
		"u_proj",
		"u_texture",
		"u_view",
		"u_position",
	};
}