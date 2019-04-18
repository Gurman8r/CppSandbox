#include <MemeGraphics/Camera.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: Transform()
		, m_position(vec3f::Zero)
	{
	}

	Camera::Camera(const Camera & copy)
		: Transform(copy)
		, m_position(copy.m_position)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Camera::rotateAround(const vec3f & target, const float speed)
	{
		// Look
		vec3f lookDir = (target - m_position).normalized();
		
		vec3f lookPos = (m_position + (target - m_position).normalized());
		
		this->lookAt(m_position, lookPos, vec3f::Up);

		// Orbit
		vec3f fwd = (lookPos - m_position);
		
		vec3f right = (fwd.cross(vec3f::Up) * vec3f(1, 0, 1)).normalized();
		
		m_position += (right * speed);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}