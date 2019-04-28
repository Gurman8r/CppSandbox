#include <MemePhysics/SphereCollider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	SphereCollider::SphereCollider()
		: m_radius(0.0f)
	{
	}

	SphereCollider::SphereCollider(const float radius)
		: m_radius(radius)
	{
	}

	SphereCollider::SphereCollider(const SphereCollider & copy)
		: m_radius(copy.m_radius)
	{
	}

	SphereCollider::~SphereCollider()
	{
	}

	bool SphereCollider::checkCollision(const Collider & other) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}