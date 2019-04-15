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
			thread().sleep(ML_PHYSICS_TIMESTEP);
			
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}