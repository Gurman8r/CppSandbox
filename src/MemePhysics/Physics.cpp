#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f Physics::Gravity(0.0f, -9.80665f, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * */

	bool Physics::beginUpdate(PhysicsState & value)
	{
		if (!m_updating)
		{
			m_updating = true;

			timer().start();
			
			mutex().lock();

			value = world().state();
			
			return true;
		}
		value = PhysicsState();
		return false;
	}

	bool Physics::endUpdate(const PhysicsState & value)
	{
		if (m_updating)
		{
			m_updating = false;
			
			mutex().unlock();

			world().state() = value;

			const Duration & elapsed = timer().stop().elapsed();

			if (elapsed.millis() < ML_PHYSICS_TIMESTEP)
			{
				thread().sleep(ML_PHYSICS_TIMESTEP - elapsed.millis());
			}
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}