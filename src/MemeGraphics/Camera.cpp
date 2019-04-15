#include <MemeGraphics/Camera.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: Camera(vec3f::Zero, quat::Identity)
	{
	}

	Camera::Camera(const vec3f & position, const quat & rotation)
		: Transform(position, vec2f::One, rotation)
		, m_position(position)
		, m_rotation(rotation)
	{
	}

	Camera::Camera(const Camera & copy)
		: Transform(copy)
		, m_position(copy.m_position)
		, m_rotation(copy.m_rotation)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}