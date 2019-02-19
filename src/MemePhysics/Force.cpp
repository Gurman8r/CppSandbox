#include <MemePhysics/Force.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f & Force::gravity(const vec3f & up, const float mass)
	{
		static vec3f f_out;

		return (f_out = (up * Physics::Gravity[1]));
	}

	const vec3f & Force::normal(const vec3f & grav, const vec3f & unorm)
	{
		static vec3f f_out;

		return (f_out = -vec3f::project(grav, unorm));
	}

	const vec3f & Force::sliding(const vec3f & grav, const vec3f & norm)
	{
		static vec3f f_out;

		return (f_out = (grav + norm));
	}

	const vec3f & Force::frictionStatic(const vec3f & norm, const vec3f & vel, const vec3f & opp, const vec3f & coeff)
	{
		static vec3f f_out;

		const vec3f max = (norm * coeff);

		return ((opp.sqrMagnitude() < max.sqrMagnitude())
			? (f_out = -(opp))
			: (f_out = -(opp.normalized() * max.magnitude())));
	}

	const vec3f & Force::frictionKinematic(const vec3f & norm, const vec3f & vel, const float coeff)
	{
		static vec3f f_out;

		const float len = (norm * coeff).magnitude();

		const vec3f n = -(vel.normalized());

		return (f_out = (n * len));
	}

	const vec3f & Force::drag(const vec3f & vel, const vec3f & fluidVel, const float fluidDensity, const float area, const float coeff)
	{
		static vec3f f_out;

		const vec3f u = ((fluidVel - vel) * (fluidVel - vel));

		f_out = (u * fluidDensity * area * coeff * 2.0f);

		return (f_out = (u * fluidDensity * area * coeff * 2.0f));
	}

	const vec3f & Force::spring(const vec3f & pos, const vec3f & anchor, const float rest, const float coeff)
	{
		static vec3f f_out;

		const vec3f diff = (pos - anchor);

		const float t = -(coeff) * (diff.magnitude() * rest);

		return (f_out = (diff * t));
	}

	const vec3f & Force::dampingLinear(const vec3f & vel, const float coeff)
	{
		static vec3f f_out;

		return (f_out = (vel * -(coeff)));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}