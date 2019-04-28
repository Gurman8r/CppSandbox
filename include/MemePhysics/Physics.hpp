#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemePhysics/PhysicsState.hpp>
#include <MemeCore/Timer.hpp>
#include <MemeCore/Thread.hpp>

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

	class Rigidbody;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Physics final
		: public IDisposable
		, public ISingleton<Physics>
	{
		friend class ISingleton<Physics>;

	public:
		static const vec3 Gravity;

	private:
		Physics();
		~Physics();

	public:
		bool dispose() override;

	public:
		bool setupRigidbody(const Rigidbody * value);

	public:
		bool beginUpdate(PhysicsState & value);
		bool endUpdate(const PhysicsState & value);

	public:
		inline const PhysicsState &	state()		const	{ return m_state;	}
		inline PhysicsState	&		state()				{ return m_state;	}
		inline const Mutex &		mutex()		const	{ return m_mutex;	}
		inline const Thread &		thread()	const	{ return m_thread;	}
		inline const Timer &		timer()		const	{ return m_timer;	}
		inline const Duration &		elapsed()	const	{ return m_elapsed; }

	public:
		template <
			class Fun,
			class ... Args
		> inline bool launch(Fun && fun, Args && ... args)
		{
			return m_thread.launch(fun, (args)...);
		}

		template <
			class Fun,
			class ... Args
		> inline void forEach(Fun && fun, Args && ... args)
		{
			PhysicsState stateCopy;
			if (beginUpdate(stateCopy))
			{
				for (int32_t i = 0, imax = stateCopy.size(); i < imax; i++)
				{
					fun(i, stateCopy, (args)...);
				}
				endUpdate(stateCopy);
			}
		}

	private:
		bool			m_updating = false;
		PhysicsState	m_state;
		Mutex			m_mutex;
		Thread			m_thread;
		Timer			m_timer;
		Duration		m_elapsed;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_HPP_