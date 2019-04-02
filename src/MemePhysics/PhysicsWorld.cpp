#include <MemePhysics/PhysicsWorld.hpp>
namespace ml
{
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
	PhysicsWorld::PhysicsState::PhysicsState()
		: position()
		, rotation()
		, transform()
		, transformInv()
	{
	}
	PhysicsWorld::PhysicsState::PhysicsState(const PhysicsState & copy)
		: position(copy.position)
		, rotation(copy.rotation)
		, transform(copy.transform)
		, transformInv(copy.transformInv)
	{
	}
	PhysicsWorld::PhysicsState::~PhysicsState()
	{
	}
}