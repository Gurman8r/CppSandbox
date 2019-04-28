#include <MemePhysics/PhysicsWorld.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Collider.hpp>
#include <MemePhysics/Particle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsWorld::PhysicsWorld()
		: m_state()
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsWorld::setupRigidbody(const Rigidbody * value)
	{
		if (value && (value->index() < m_state.size()))
		{
			if (const Transform * transform = value->transform())
			{
				if (m_state.setPos(value->index(), transform->getPos()) &&
					m_state.setRot(value->index(), transform->getRot()))
				{
					return true;
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}