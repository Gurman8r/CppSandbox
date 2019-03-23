#ifndef _ML_PARTICLE_HPP_
#define _ML_PARTICLE_HPP_

#include <MemePhysics/Force.hpp>

namespace ml
{
	struct ML_PHYSICS_API Particle
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec3f pos;
		vec3f vel;
		vec3f acc;
		vec3f force;
		float mass;
		float massInv;

		/* * * * * * * * * * * * * * * * * * * * */

		Particle();
		Particle(const vec3f & pos, const float mass);
		Particle(const vec3f & pos, const vec3f & vel, const vec3f & acc, const float mass);
		Particle(const Particle & copy);
		~Particle();

		/* * * * * * * * * * * * * * * * * * * * */

		bool isMoving() const;

		Particle & applyForce(const vec3f & value);
		Particle & convertForce();
		Particle & resetForce();
		Particle & setMass(float value);

		/* * * * * * * * * * * * * * * * * * * * */

		Particle & integrateEulerExplicit(const float dt);
		Particle & integrateEulerSemiImplicit(const float dt);
		Particle & integrateEulerKinematic(const float dt);

		/* * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ML_PARTICLE_HPP_