#ifndef _ML_PHYSICS_WORLD_HPP_
#define _ML_PHYSICS_WORLD_HPP_

#include <MemeCore/Vector3.hpp>
#include <MemeCore/Quaternion.hpp>
#include <MemeCore/Timer.hpp>
#include <MemePhysics/Export.hpp>

namespace ml 
{
	class ML_PHYSICS_API PhysicsWorld
		: public ITrackable
	{
	public:
		struct PhysicsState 
		{
			List<vec3f> position;
			List<quat> rotation;
			List<mat4f> transform, transformInv;

			PhysicsState();
			PhysicsState(const PhysicsState &copy);
			~PhysicsState();
		};
	public:
		PhysicsWorld();
		~PhysicsWorld();

	public:
		PhysicsState state;

		Timer timer; //ms

		std::thread *thread;

		bool lock, init;
	};

	
}
#endif // !_ML_PHYSICS_WORLD_HPP_