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
		, public INonCopyable
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

	public:
		inline PhysicsState & state() { return m_state; }

	private:
		PhysicsState m_state;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_PHYSICS_WORLD_HPP_