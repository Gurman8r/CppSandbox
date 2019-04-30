#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

#include <MemePhysics/PhysicsState.hpp>
#include <MemeCore/Timer.hpp>
#include <MemeCore/Thread.hpp>

#define ML_Physics ml::Physics::getInstance()

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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static const vec3 Gravity;

		static constexpr Duration TimeStep { static_cast<Milliseconds>(15) };

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Physics();
		~Physics();


	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool dispose() override;


	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool createLinkToRigidbody(Rigidbody * rb);
		Rigidbody * createNewRigidbody(const Rigidbody & copy);
		Rigidbody * getLinkedRigidbody(const int32_t index) const;


	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool beginUpdate(PhysicsState & value);
		bool endUpdate(const PhysicsState & value);


	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
		> inline bool updateAll(Fun && fun, Args && ... args)
		{
			PhysicsState temp;
			if (beginUpdate(temp))
			{
				for (int32_t i = 0, imax = temp.size(); i < imax; i++)
				{
					fun(i, temp, (args)...);
				}
				return endUpdate(temp);
			}
			return false;
		}

		template <
			class Fun, 
			class ... Args
		> inline bool getCopyState(Fun && fun, Args && ... args)
		{
			PhysicsState temp;
			if (!m_updating && temp.deepCopy(m_state))
			{
				fun(temp, (args)...);
			}
			return (bool)(temp);
		}
		
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline const PhysicsState &	state()		const	{ return m_state;	}
		inline const Mutex &		mutex()		const	{ return m_mutex;	}
		inline const Thread &		thread()	const	{ return m_thread;	}
		inline const Timer &		timer()		const	{ return m_timer;	}
		inline const Duration &		elapsed()	const	{ return m_elapsed; }

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		bool			m_updating;
		PhysicsState	m_state;
		Mutex			m_mutex;
		Thread			m_thread;
		Timer			m_timer;
		Duration		m_elapsed;

		List<Rigidbody *> m_rb;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_HPP_