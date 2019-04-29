#include <MemePhysics/SphereCollider.hpp>
#include <MemePhysics/BoxCollider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SphereCollider::SphereCollider()
		: Collider(Collider::T_Sphere)
		, m_radius(0.0f)
	{
	}

	SphereCollider::SphereCollider(const float radius)
		: Collider(Collider::T_Sphere)
		, m_radius(radius)
	{
	}

	SphereCollider::SphereCollider(const SphereCollider & copy)
		: Collider(Collider::T_Sphere)
		, m_radius(copy.m_radius)
	{
	}

	SphereCollider::~SphereCollider()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool SphereCollider::checkCollision(const Collider & other) const
	{
		switch (other.getHullType())
		{
		case Collider::T_Box:
			return false;

		case Collider::T_Sphere:
			return false;

		default:
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}