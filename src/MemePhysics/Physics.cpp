#include <MemePhysics/Physics.hpp>

#define ML_GRAVITY -9.80665f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f Physics::Gravity(0.0f, ML_GRAVITY, 0.0f);

	/* * * * * * * * * * * * * * * * * * * * */

	bool Physics::beginUpdate(PhysicsState & value)
	{
		if (!m_updating)
		{
			m_updating = true;

			value = world().state();
			
			timer().start();

			mutex().lock();
			
			return true;
		}
		return false;
	}

	bool Physics::endUpdate(const PhysicsState & value)
	{
		if (m_updating)
		{
			m_updating = false;
			
			mutex().unlock();

			const Duration & elapsed = timer().stop().elapsed();

			world().state() = value;

			if (elapsed.milliseconds() < ML_PHYSICS_TIMESTEP)
			{
				thread().sleep(ML_PHYSICS_TIMESTEP - elapsed.milliseconds());
			}
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}