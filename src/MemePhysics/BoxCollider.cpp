#include <MemePhysics/BoxCollider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	BoxCollider::BoxCollider()
		: m_size(0.0f)
	{
	}

	BoxCollider::BoxCollider(const vec3 & size)
		: m_size(size)
	{
	}

	BoxCollider::BoxCollider(const BoxCollider & copy)
		: m_size(copy.m_size)
	{
	}

	BoxCollider::~BoxCollider()
	{
	}

	bool BoxCollider::checkCollision(const Collider & other) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}