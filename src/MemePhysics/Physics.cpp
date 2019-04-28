#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Collider.hpp>
#include <MemePhysics/Particle.hpp>

#define ML_GRAVITY -9.80665f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3 Physics::Gravity(0.0f, ML_GRAVITY, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Physics::Physics() 
	{
	}
	
	Physics::~Physics() 
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::dispose()
	{
		return m_thread.dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::setupState(const size_t count)
	{
		return m_state.resize(count);
	}

	bool Physics::setupRigidbody(const Rigidbody * value)
	{
		int32_t i;
		if (value && ((i = value->index()) < m_state.size()))
		{
			if (const Transform * transform = value->transform())
			{
				if (m_state.set<PhysicsState::T_Pos>(i, transform->getPos()) &&
					m_state.set<PhysicsState::T_Rot>(i, transform->getRot()) &&
					m_state.set<PhysicsState::T_Mat>(i, transform->getMat()) &&
					m_state.set<PhysicsState::T_Inv>(i, transform->getInv()))
				{
					return true;
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::beginUpdate(PhysicsState & value)
	{
		if (!m_updating)
		{
			m_updating = true;

			value.deepCopy(m_state);
			
			m_timer.start();

			m_mutex.lock();
			
			return true;
		}
		return false;
	}

	bool Physics::endUpdate(const PhysicsState & value)
	{
		if (m_updating)
		{
			m_updating = false;
			
			m_mutex.unlock();

			m_elapsed = m_timer.stop().elapsed();

			m_state.deepCopy(value);

			if (m_elapsed.milliseconds() < ML_PHYSICS_TIMESTEP)
			{
				m_thread.sleep(ML_PHYSICS_TIMESTEP - m_elapsed.milliseconds());
			}
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}