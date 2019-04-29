#include <MemePhysics/Particle.hpp>
#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	//Melody: (and me) update these as well
	Particle::Particle()
		: pos		(vec3::Zero)
		, vel		(vec3::Zero)
		, acc		(vec3::Zero)
		, force		(vec3::Zero)
		, mass		(0.0f)
		, massInv	(0.0f)
	{
	}

	Particle::Particle(const vec3 & pos, const float mass)
		: Particle(pos, vec3::Zero, vec3::Zero, mass)
	{
	}

	Particle::Particle(const vec3 & pos, const vec3 & vel, const vec3 & acc, const float mass)
		: pos		(pos)
		, vel		(vel)
		, acc		(acc)
		, force		(vec3::Zero)
	{
		setMass(mass);
	}

	Particle::Particle(const Particle & copy)
		: pos		(copy.pos)
		, vel		(copy.vel)
		, acc		(copy.acc)
		, force		(copy.force)
		, mass		(copy.mass)
		, massInv	(copy.massInv)
	{
	}

	Particle::~Particle()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Particle::isMoving() const
	{
		return (vel.sqrMagnitude() != 0.0f);
	}

	Particle & Particle::applyForce(const vec3 & value)
	{
		force += value;
		return (*this);
	}

	Particle & Particle::convertForce()
	{
		acc += (force / mass);
		return (*this);
	}

	Particle & Particle::resetForce()
	{
		force = vec3::Zero;
		return (*this);
	}

	Particle & Particle::setMass(float value)
	{
		if (value == 0.0f) { value = FLT_MIN; }
		mass = value;
		massInv = (1.0f / value);
		return (*this);
	}

	Particle & Particle::reset()
	{
		// TODO: insert return statement here
		pos = 0.0f;
		vel = 0.0f;
		acc = 0.0f;
		momentum = 0.0f;
		force = 0.0f;
		mass = massInv = 0.0f;
		rotation = glm::quat();
		angularVel = 0.0f;
		angularAcc = 0.0f;
		angularMomentum = 0.0f;
		torque = 0.0f;
		inertiaTensor = inertiaTensorInv = mat3::Identity();
		inertiaTensor_world = inertiaTensorInv_world = mat3::Identity();
		centerMass = 0.0f;
		centerMass_world = 0.0f;

		return (*this);
	}

	bool Particle::isRotating()
	{
		// TODO: insert return statement here
		return (angularVel.sqrMagnitude() != 0.0f);
	}

	Particle & Particle::applyForceLocation(const vec3 & force, const vec3 worldLoc)
	{
		// TODO: insert return statement here
		torque += vec3::cross((worldLoc - centerMass_world), force);
		return (*this);

	}

	Particle & Particle::convertTorque()
	{
		// TODO: insert return statement here
		//Melody: how do you multiply a matrix and a vector?

		//angularAcc = 
		angularAcc = (glm::mat3)(inertiaTensorInv_world) * (glm::vec3)(torque);
		return (*this);
	}

	Particle & Particle::resetTorque()
	{
		// TODO: insert return statement here
		torque = 0.0f;

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	//Melody: (and me) these need to be updated to include rotational

	vec3 Particle::rotateForce(Particle * p, vec3 force)
	{
		vec3 wv = force * p->rotation[3];
		//Melody: (and me) check this once I'm sure how cross works

		return vec3();
	}

	Particle & Particle::integrateEulerExplicit(const float dt)
	{
		pos += (vel * dt);
		vel += (acc * dt);
		return (*this);
	}

	Particle & Particle::integrateEulerSemiImplicit(const float dt)
	{
		vel += (acc * dt);
		pos += (vel * dt);
		return (*this);
	}

	Particle & Particle::integrateEulerKinematic(const float dt)
	{
		pos += ((vel * dt) + (acc * (dt * dt * 0.5f)));
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}