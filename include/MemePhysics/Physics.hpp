#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

#include <MemePhysics/Particle.hpp>
#include <MemePhysics/PhysicsWorld.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Physics ml::Physics::getInstance()

#define ML_PHYSICS_TIMESTEP ml::Millis(30)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Physics final
		: public ISingleton<Physics>
	{
		friend class ISingleton<Physics>;

	public:
		static const vec3f Gravity;

	public:
		bool beginUpdate(PhysicsState & value);
		bool endUpdate(const PhysicsState & value);

	public:
		inline Mutex		& mutex()	{ return m_mutex;	}
		inline Thread		& thread()	{ return m_thread;	}
		inline PhysicsWorld & world()	{ return m_world;	}

	private:
		Mutex m_mutex;
		Thread m_thread;
		PhysicsWorld m_world;

		mutable bool m_updating = false;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_HPP_