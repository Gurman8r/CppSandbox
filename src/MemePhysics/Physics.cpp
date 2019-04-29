#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Collider.hpp>
#include <MemePhysics/Particle.hpp>
#include <MemeCore/Debug.hpp>

#define ML_GRAVITY -9.80665f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3 Physics::Gravity(0.0f, ML_GRAVITY, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	Physics::Physics() 
		: m_updating(false)
		, m_state	()
		, m_mutex	()
		, m_thread	()
		, m_timer	()
		, m_elapsed	()
		, m_rb		()
	{
	}
	
	Physics::~Physics() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::dispose()
	{
		return m_thread.dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Physics::createLinkToRigidbody(const Rigidbody * value)
	{
		int32_t i;
		if (value && ((i = value->index()) >= 0))
		{
			if (const Transform * transform = value->transform())
			{
				if (m_state.push<PhysicsState::T_Pos>(vec3()) == i &&
					m_state.push<PhysicsState::T_Rot>(quat()) == i &&
					m_state.push<PhysicsState::T_Mat>(mat4()) == i &&
					m_state.push<PhysicsState::T_Inv>(mat4()) == i)
				{
					if (m_state.set<PhysicsState::T_Pos>(i, transform->getPos()) &&
						m_state.set<PhysicsState::T_Rot>(i, transform->getRot()) &&
						m_state.set<PhysicsState::T_Mat>(i, transform->getMat()) &&
						m_state.set<PhysicsState::T_Inv>(i, transform->getInv()))
					{
						m_rb.push_back(value);
						
						return ((m_state.m_size = (int32_t)m_rb.size()) > 0);
					}
					else
					{
						return Debug::logError("Failed setting RB data");
					}
				}
				else
				{
					return Debug::logError("Failed pushing RB data");
				}
			}
			else
			{
				return Debug::logError("Failed getting RB transform");
			}
		}
		else
		{
			return Debug::logError("Invalid RB index");
		}
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