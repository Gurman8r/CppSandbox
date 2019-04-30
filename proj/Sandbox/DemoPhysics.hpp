#ifndef _DEMO_PHYSICS_HPP_
#define _DEMO_PHYSICS_HPP_

#include <MemePhysics/Physics.hpp>

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum Rigidbody_ID : int32_t
	{
		RB_BORG,
		RB_CUBE,
		RB_EARTH,
		RB_GROUND,
		RB_MOON,
		RB_NAVBALL,

		MAX_DEMO_RIGIDBODY
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct demo_physics final
	{
		static void init();
		
		static void update(const int32_t i, ml::PhysicsState & state);
		
		static void sync(const ml::PhysicsState & state);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_DEMO_PHYSICS_HPP_