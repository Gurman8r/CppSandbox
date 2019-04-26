#include <MemeGraphics/Camera.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: resolution(vec2i::Zero)
		, position(vec3f::Zero)
		, transform()
		, ortho()
		, persp()
		, fieldOfView(45.0f)
		, perspNear(0.1f)
		, perspFar(1000.0f)
		, orthoNear(-1.0f)
		, orthoFar(+1.0f)
	{
	}

	Camera::Camera(float fieldOfView, float pNear, float pFar, float oNear, float oFar)
		: resolution(vec2i::Zero)
		, position(vec3f::Zero)
		, transform()
		, ortho()
		, persp()
		, fieldOfView(fieldOfView)
		, perspNear(pNear)
		, perspFar(pFar)
		, orthoNear(oNear)
		, orthoFar(oFar)
	{
	}

	Camera::Camera(const Camera & copy)
		: resolution(copy.resolution)
		, position(copy.position)
		, transform(copy.transform)
		, ortho(copy.ortho)
		, persp(copy.persp)
		, fieldOfView(copy.fieldOfView)
		, perspNear(copy.perspNear)
		, perspFar(copy.perspFar)
		, orthoNear(copy.orthoNear)
		, orthoFar(copy.orthoFar)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::update(const vec2i & value)
	{
		if ((value != vec2i::Zero) && (resolution != value))
		{
			resolution = value;

			// Orthographic
			ortho.update(Transform::Orthographic(
				{ vec2f::Zero, (vec2f)(resolution) },
				{ orthoNear, orthoFar }
			));

			// Perspective
			persp.update(Transform::Perspective(
				fieldOfView, ((float)resolution[0] / (float)resolution[1]),
				perspNear, perspFar
			));
		}
		return (*this);
	}

	Camera & Camera::lookDir(const vec3f & dir)
	{
		transform.lookAt(position, dir, vec3f::Up);
		return (*this);
	}

	Camera & Camera::lookAt(const vec3f & target)
	{
		return lookDir(position + target);
	}

	Camera & Camera::orbit(const vec3f & target, const float speed)
	{
		// Look
		vec3f lookPos = (position + (target - position).normalized());
		transform.lookAt(position, lookPos, vec3f::Up);

		// Move
		vec3f fwd = (lookPos - position);
		vec3f right = (fwd.cross(vec3f::Up) * vec3f(1, 0, 1)).normalized();
		position += (right * speed);

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}