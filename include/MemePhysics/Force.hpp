#ifndef _ML_FORCE_HPP_
#define _ML_FORCE_HPP_

#include <MemePhysics/Physics.hpp>

namespace ml
{
	class ML_PHYSICS_API Force
	{
	public:
		static const vec3f gravity(const vec3f & up, const float mass);
		static const vec3f normal(const vec3f & grav, const vec3f & unorm);
		static const vec3f sliding(const vec3f & grav, const vec3f & norm);
		static const vec3f frictionStatic(const vec3f & norm, const vec3f & vel, const vec3f & opp, const vec3f & coeff);
		static const vec3f frictionKinematic(const vec3f & norm, const vec3f & vel, const float coeff);
		static const vec3f drag(const vec3f & vel, const vec3f & fluidVel, const float fluidDensity, const float area, const float coeff);
		static const vec3f spring(const vec3f & pos, const vec3f & anchor, const float rest, const float coeff);
		static const vec3f dampingLinear(const vec3f & vel, const float coeff);
	};
}

#endif // !_ML_FORCE_HPP_