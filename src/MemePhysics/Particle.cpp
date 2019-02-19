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

	Particle::~Particle()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Particle::isMoving() const
	{
		return vel.sqrMagnitude() != 0.0f;
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
		mass = value;
		massInv = 1.0f / value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}