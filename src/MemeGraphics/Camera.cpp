#include <MemeGraphics/Camera.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: position		(vec3f::Zero)
		, resolution	(vec2i::Zero)
		, transform		()
		, ortho			()
		, persp			()
		, fieldOfView	(45.0f)
		, perspNear		(0.1f)
		, perspFar		(1000.0f)
		, orthoNear		(-1.0f)
		, orthoFar		(+1.0f)
	{
	}

	Camera::Camera(const Camera & copy)
		: resolution	(copy.resolution)
		, position		(copy.position)
		, transform		(copy.transform)
		, ortho			(copy.ortho)
		, persp			(copy.persp)
		, fieldOfView	(copy.fieldOfView)
		, perspNear		(copy.perspNear)
		, perspFar		(copy.perspFar)
		, orthoNear		(copy.orthoNear)
		, orthoFar		(copy.orthoFar)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Camera::update(const vec2i & value)
	{
		if ((value != vec2i::Zero) && (resolution != value))
		{
			resolution = value;

			// Orthographic
			ortho = Transform::Orthographic(
				{ vec2f::Zero, (vec2f)(resolution) },
				{ orthoNear, orthoFar }
			);

			// Perspective
			persp = Transform::Perspective(
				fieldOfView, ((float)resolution[0] / (float)resolution[1]),
				perspNear, perspFar
			);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Camera::orbit(const vec3f & target, const float speed)
	{
		// Look
		vec3f lookDir = (target - position).normalized();
		
		vec3f lookPos = (position + (target - position).normalized());
		
		transform = Transform::LookAt(position, lookPos, vec3f::Up);
		
		// Move
		vec3f fwd = (lookPos - position);
		
		vec3f right = (fwd.cross(vec3f::Up) * vec3f(1, 0, 1)).normalized();
		
		position += (right * speed);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}