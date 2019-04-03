#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

#include <MemeCore/Thread.hpp>
#include <MemePhysics/Force.hpp>
#include <MemePhysics/Particle.hpp>
#include <MemePhysics/PhysicsWorld.hpp>

#define ML_Physics ml::Physics::getInstance()

namespace ml
{
	class ML_PHYSICS_API Physics final
		: public ISingleton<Physics>
	{
		friend class ISingleton<Physics>;

	public:
		static const vec3f Gravity;

	public:
		inline Thread & thread() { return m_thread; }

		inline PhysicsWorld & world() { return m_world; }

	private:
		Thread m_thread;
		PhysicsWorld m_world;
	};
}

#endif // !_ML_PHYSICS_HPP_