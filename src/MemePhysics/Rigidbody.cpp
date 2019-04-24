#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Rigidbody::Rigidbody()
		: m_index(PhysicsState::InvalidIndex)
		, m_transform(NULL)
	{
	}

	Rigidbody::Rigidbody(int32_t index, Transform * transform)
		: m_index(index)
		, m_transform(transform)
	{
	}

	Rigidbody::Rigidbody(const Rigidbody & copy)
		: m_index(copy.m_index)
		, m_transform(copy.m_transform)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}