#include <MemeGraphics/Light.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Light::Light()
		: position(vec3f::Zero)
		, color(Color::White)
	{
	}

	Light::Light(const vec3f & position, const vec4f & color)
		: position(position)
		, color(color)
	{
	}

	Light::Light(const Light & copy)
		: position(copy.position)
		, color(copy.color)
	{
	}

	Light::~Light()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}