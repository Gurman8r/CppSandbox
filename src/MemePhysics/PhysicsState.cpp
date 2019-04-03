#include <MemePhysics/PhysicsState.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState::PhysicsState()
		: position()
		, rotation()
		, transform()
		, transformInv()
	{
	}

	PhysicsState::PhysicsState(const PhysicsState & copy)
		: position(copy.position)
		, rotation(copy.rotation)
		, transform(copy.transform)
		, transformInv(copy.transformInv)
	{
	}

	PhysicsState::~PhysicsState()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}