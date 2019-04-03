#ifndef _ML_PHYSICS_WORLD_HPP_
#define _ML_PHYSICS_WORLD_HPP_

#include <MemePhysics/PhysicsState.hpp>
#include <MemeCore/Timer.hpp>
#include <MemeCore/Thread.hpp>

namespace ml 
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API PhysicsWorld
		: public ITrackable
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

	public:
		inline PhysicsState & state() { return m_state; }
		inline Timer		& timer() { return m_timer; }

	private:
		PhysicsState	m_state;
		Timer			m_timer;
		Thread			m_thread;
		bool			m_lock;
		bool			m_init;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_PHYSICS_WORLD_HPP_