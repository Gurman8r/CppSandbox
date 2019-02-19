#ifndef _PHYSICS_HPP_
#define _PHYSICS_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Vector3.hpp>

namespace ml
{
	class ML_PHYSICS_API Physics final
	{
	public:
		static const vec3f Gravity;

	public:
		static const vec3f & forceGravity(const vec3f & up, const float mass);

		static const vec3f & forceNormal(const vec3f & grav, const vec3f & unorm);

		static const vec3f & forceSliding(const vec3f & grav, const vec3f & norm);

		static const vec3f & forceFrictionStatic(const vec3f & norm, const vec3f & vel, const vec3f & opp, const vec3f & coeff);

		static const vec3f & forceFrictionKinematic();
	};
}

#endif // !_PHYSICS_HPP_
