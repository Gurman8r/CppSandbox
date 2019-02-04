#include <MemeGraphics/Camera.hpp>

namespace ml
{
	Camera::Camera()
		: Transform()
	{
	}

	Camera::Camera(const vec3f & position, const vec3f & scale, const quat & rotation)
		: Transform(position, scale, rotation)
	{
	}

	Camera::Camera(const mat4f & value)
		: Transform(value)
	{
	}

	Camera::Camera(const Camera & copy)
		: Transform(copy)
	{
	}

	Camera::~Camera()
	{
	}
}