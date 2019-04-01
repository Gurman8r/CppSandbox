#ifndef _ML_PHYSICS_HPP_
#define _ML_PHYSICS_HPP_

#include <MemePhysics/Force.hpp>
#include <MemePhysics/Particle.hpp>

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
		std::vector<Particle> particles;
	};
}

#endif // !_ML_PHYSICS_HPP_