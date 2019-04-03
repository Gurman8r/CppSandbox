#include <MemePhysics/PhysicsWorld.hpp>
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsWorld::PhysicsWorld()
		: state()
		, timer()
		, thread()
		, lock(false)
		, init(false)
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}