#include <MemePhysics/Particle.hpp>
#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	//Melody: (and me) update these as well
	Particle::Particle()
		: pos		(vec3f::Zero)
		, vel		(vec3f::Zero)
		, acc		(vec3f::Zero)
		, force		(vec3f::Zero)
		, mass		(0.0f)
		, massInv	(0.0f)
	{
	}

	Particle::Particle(const vec3f & pos, const float mass)
		: Particle(pos, vec3f::Zero, vec3f::Zero, mass)
	{
	}

	Particle::Particle(const vec3f & pos, const vec3f & vel, const vec3f & acc, const float mass)
		: pos		(pos)
		, vel		(vel)
		, acc		(acc)
		, force		(vec3f::Zero)
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

	Particle & Particle::applyForce(const vec3f & value)
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
		force = vec3f::Zero;
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
		return (*this);
	}

	bool Particle::isRotating()
	{
		// TODO: insert return statement here
		return (angularVel.sqrMagnitude() != 0.0f);
	}

	Particle & Particle::applyForceLocation(const vec3f & force, const vec3f worldLoc)
	{
		// TODO: insert return statement here
		vec3f arm = worldLoc - centerMass_world;
		torque += arm.cross(force);		//Melody: please make sure I'm using cross right
		return (*this);

	}

	Particle & Particle::convertTorque()
	{
		// TODO: insert return statement here
		//Melody: how do you multiply a matrix and a vector?

		//angularAcc = 
		return (*this);
	}

	Particle & Particle::resetTorque()
	{
		// TODO: insert return statement here
		torque = { 0.0f,0.0f,0.0f };

	}

	/* * * * * * * * * * * * * * * * * * * * */

	//Melody: (and me) these need to be updated to include rotational

	vec3f Particle::rotateForce(Particle * p, vec3f force)
	{
		vec3f wv = force * p->rotation[3];
		//Melody: (and me) check this once I'm sure how cross works

		return vec3f();
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