#include <MemePhysics/PhysicsWorld.hpp>
namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsWorld::PhysicsWorld()
		: m_state()
		, m_timer()
		, m_thread()
		, m_lock(false)
		, m_init(false)
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}