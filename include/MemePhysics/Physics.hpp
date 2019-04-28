#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemePhysics/PhysicsWorld.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Physics ml::Physics::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

# define ML_PHYSICS_TIMESTEP_15 60
# define ML_PHYSICS_TIMESTEP_30 30
# define ML_PHYSICS_TIMESTEP_60 15

# ifndef ML_PHYSICS_TIMESTEP
# define ML_PHYSICS_TIMESTEP ML_PHYSICS_TIMESTEP_60
# endif

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Physics final
		: public ISingleton<Physics>
	{
		friend class ISingleton<Physics>;

	private:
		Physics() {}
		~Physics() {}

	public:
		static const vec3 Gravity;

	public:
		bool beginUpdate(PhysicsState & value);
		bool endUpdate(const PhysicsState & value);

	public:
		inline Mutex		& mutex()	{ return m_mutex;	}
		inline Thread		& thread()	{ return m_thread;	}
		inline Timer		& timer()	{ return m_timer;	}
		inline PhysicsWorld & world()	{ return m_world;	}

	private:
		bool			m_updating = false;
		Mutex			m_mutex;
		Thread			m_thread;
		Timer			m_timer;
		PhysicsWorld	m_world;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_HPP_