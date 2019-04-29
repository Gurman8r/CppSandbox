#ifndef _ML_PARTICLE_HPP_
#define _ML_PARTICLE_HPP_

#include <MemePhysics/Force.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_PHYSICS_API Particle final
		: public ITrackable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		vec3 pos;
		vec3 vel;
		vec3 acc;
		vec3 force;
		vec3 momentum;
		float mass;
		float massInv;

		//angular
		quat rotation;
		vec3 angularVel;
		vec3 angularAcc;
		vec3 angularMomentum;
		vec3 torque;
		mat4 inertiaTensor, inertiaTensorInv;
		mat4 inertiaTensor_world, inertiaTensorInv_world;
		vec3 centerMass;
		vec3 centerMass_world;

		/* * * * * * * * * * * * * * * * * * * * */

		Particle();
		Particle(const vec3 & pos, const float mass);
		Particle(const vec3 & pos, const vec3 & vel, const vec3 & acc, const float mass);
		Particle(const Particle & copy);
		~Particle();

		/* * * * * * * * * * * * * * * * * * * * */

		bool isMoving() const;

		Particle & applyForce(const vec3 & value);
		Particle & convertForce();
		Particle & resetForce();
		Particle & setMass(float value);
		Particle & reset();
		bool isRotating();
		Particle & applyForceLocation(const vec3 & force, const vec3 worldLoc);
		Particle & convertTorque();
		Particle & resetTorque();

		/* * * * * * * * * * * * * * * * * * * * */
		//inertia Tensor Stuff
		//put stuff here. Ask melody how she wants to do it.
		//dans are column major, melodys are row. swap em. Or, make them as glm matrices then convert: probably better.

		/* * * * * * * * * * * * * * * * * * * * */
		static vec3 rotateForce(Particle *p, vec3 force);


		/* * * * * * * * * * * * * * * * * * * * */


		Particle & integrateEulerExplicit(const float dt);
		Particle & integrateEulerSemiImplicit(const float dt);
		Particle & integrateEulerKinematic(const float dt);

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PARTICLE_HPP_