#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f Physics::Gravity(0.0f, -9.80665f, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f & Physics::forceGravity(const vec3f & up, const float mass)
	{
		static vec3f f_out;
		return (f_out = (up * Gravity[1]));
	}

	const vec3f & Physics::forceNormal(const vec3f & grav, const vec3f & unorm)
	{
		static vec3f f_out;
		return (f_out = -vec3f::project(grav, unorm));
	}

	const vec3f & Physics::forceSliding(const vec3f & grav, const vec3f & norm)
	{
		static vec3f f_out;
		return (f_out = (grav + norm));
	}

	const vec3f & Physics::forceFrictionStatic(const vec3f & norm, const vec3f & vel, const vec3f & opp, const vec3f & coeff)
	{
		static vec3f f_out;
		if(opp.sqrMagnitude())
		{
			
		}
		return f_out;
	}

	const vec3f & Physics::forceFrictionKinematic()
	{
		static vec3f f_out;
		return f_out;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}