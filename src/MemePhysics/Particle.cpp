#include <MemePhysics/Particle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
		: pos		(pos)
		, vel		(vec3f::Zero)
		, acc		(vec3f::Zero)
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

	/* * * * * * * * * * * * * * * * * * * * */

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