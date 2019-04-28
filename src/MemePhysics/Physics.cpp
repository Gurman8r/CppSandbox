#include <MemePhysics/Physics.hpp>

#define ML_GRAVITY -9.80665f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3 Physics::Gravity(0.0f, ML_GRAVITY, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * */

	bool Physics::beginUpdate(PhysicsState & value)
	{
		if (!m_updating)
		{
			m_updating = true;

			value = m_world.state();
			
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

			m_world.state() = value;

			const Duration & elapsed = m_timer.stop().elapsed();

			if (elapsed.milliseconds() < ML_PHYSICS_TIMESTEP)
			{
				m_thread.sleep(ML_PHYSICS_TIMESTEP - elapsed.milliseconds());
			}
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}