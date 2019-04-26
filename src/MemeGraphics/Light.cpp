#include <MemeGraphics/Light.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Light::Light()
		: color(Color::White)
	{
	}

	Light::Light(const vec4f & color)
		: color(color)
	{
	}

	Light::Light(const Light & copy)
		: color(copy.color)
	{
	}

	Light::~Light()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}