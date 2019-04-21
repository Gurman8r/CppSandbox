#include <MemeGraphics/Camera.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: m_position	(vec3f::Zero)
		, m_resolution	(vec2i::Zero)
		, m_transform	()
		, m_ortho		()
		, m_persp		()
		, m_fieldOfView	(45.0f)
		, m_perspNear	(0.1f)
		, m_perspFar	(1000.0f)
		, m_orthoNear	(-1.0f)
		, m_orthoFar	(+1.0f)
	{
	}

	Camera::Camera(const Camera & copy)
		: m_position	(copy.m_position)
		, m_resolution	(copy.m_resolution)
		, m_transform	(copy.m_transform)
		, m_ortho		(copy.m_ortho)
		, m_persp		(copy.m_persp)
		, m_fieldOfView	(copy.m_fieldOfView)
		, m_perspNear	(copy.m_perspNear)
		, m_perspFar	(copy.m_perspFar)
		, m_orthoNear	(copy.m_orthoNear)
		, m_orthoFar	(copy.m_orthoFar)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Camera::update(const vec2i & resolution)
	{
		if ((m_resolution != resolution) && ((resolution[0] >= 0) && (resolution[1] >= 0)))
		{
			m_resolution = resolution;

			// Orthographic
			m_ortho.orthographic(
				{ vec2f::Zero, (vec2f)(resolution) },
				{ m_orthoNear, m_orthoFar }
			);

			// Perspective
			m_persp.perspective(
				m_fieldOfView, ((float)resolution[0] / (float)resolution[1]),
				m_perspNear, m_perspFar
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Camera::orbit(const vec3f & target, const float speed)
	{
		// Look
		vec3f lookDir = (target - m_position).normalized();
		
		vec3f lookPos = (m_position + (target - m_position).normalized());
		
		m_transform.lookAt(m_position, lookPos, vec3f::Up);

		// Orbit
		vec3f fwd = (lookPos - m_position);
		
		vec3f right = (fwd.cross(vec3f::Up) * vec3f(1, 0, 1)).normalized();
		
		m_position += (right * speed);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}