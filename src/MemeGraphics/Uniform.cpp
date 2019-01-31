#include <MemeGraphics/Uniform.h>

namespace ml
{
	const string Uniform::Names[Uniform::ID::MAX_UNIFORM_ID] =
	{
		"u_model",
		"u_proj",
		"u_view",
		
		"u_color",
		"u_texture",

		"u_curveMode",
	};
}