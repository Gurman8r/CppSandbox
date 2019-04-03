#ifndef _ML_PHYSICS_WORLD_HPP_
#define _ML_PHYSICS_WORLD_HPP_

#include <MemePhysics/PhysicsState.hpp>
#include <MemeCore/Timer.hpp>

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
		PhysicsState	state;
		Timer			timer;
		std::thread *	thread;
		bool			lock;
		bool			init;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_PHYSICS_WORLD_HPP_