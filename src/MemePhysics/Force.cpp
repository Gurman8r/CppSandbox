#include <MemePhysics/Force.hpp>
#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f Force::gravity(const vec3f & up, const float mass)
	{
		return (up * Physics::Gravity[1]);
	}

	const vec3f Force::normal(const vec3f & grav, const vec3f & unorm)
	{
		return -(vec3f::project(grav, unorm));
	}

	const vec3f Force::sliding(const vec3f & grav, const vec3f & norm)
	{
		return (grav + norm);
	}

	const vec3f Force::frictionStatic(const vec3f & norm, const vec3f & vel, const vec3f & opp, const vec3f & coeff)
	{
		const vec3f max = (norm * coeff);
		
		return ((opp.sqrMagnitude() < max.sqrMagnitude())
			? -(opp)
			: -(opp.normalized() * max.magnitude()));
	}

	const vec3f Force::frictionKinematic(const vec3f & norm, const vec3f & vel, const float coeff)
	{
		const float len = (norm * coeff).magnitude();

		const vec3f n = -(vel.normalized());

		return (n * len);
	}

	const vec3f Force::drag(const vec3f & vel, const vec3f & fluidVel, const float fluidDensity, const float area, const float coeff)
	{
		const vec3f u = (fluidVel - vel);

		return ((u * u) * fluidDensity * area * coeff * 2.0f);
	}

	const vec3f Force::spring(const vec3f & pos, const vec3f & anchor, const float rest, const float coeff)
	{
		const vec3f diff = (pos - anchor);

		const float t = -(coeff) * (diff.magnitude() * rest);

		return (diff * t);
	}

	const vec3f Force::dampingLinear(const vec3f & vel, const float coeff)
	{
		return (vel * -(coeff));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}