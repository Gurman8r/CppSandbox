#include <MemePhysics/PhysicsWorld.hpp>
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsWorld::PhysicsWorld()
		: state()
		, timer()
		, thread(NULL)
		, lock(false)
		, init(false)
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}