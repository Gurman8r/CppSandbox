#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Collider.hpp>
#include <MemePhysics/Particle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Rigidbody::Rigidbody()
		: m_index		(-1)
		, m_transform	(NULL)
		, m_collider	(NULL)
		, m_particle	(NULL)
		, m_linked		(false)
	{
	}

	Rigidbody::Rigidbody(int32_t index, Transform * transform, Collider * collider, Particle * particle)
		: m_index		(index)
		, m_transform	(transform)
		, m_collider	(collider)
		, m_particle	(particle)
		, m_linked		(false)
	{
	}

	Rigidbody::Rigidbody(const Rigidbody & copy)
		: m_index		(copy.m_index)
		, m_transform	(copy.m_transform)
		, m_collider	(copy.m_collider)
		, m_particle	(copy.m_particle)
		, m_linked		(copy.m_linked)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Rigidbody::createLinkToWorld() const
	{
		return ((!m_linked)
			? (m_linked = ML_Physics.createLinkToRigidbody(this))
			: (false)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}