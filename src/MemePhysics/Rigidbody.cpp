#include <MemePhysics/Rigidbody.hpp>
#include <MemePhysics/Physics.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Rigidbody::Rigidbody()
		: m_transform(NULL)
	{
	}

	Rigidbody::Rigidbody(Transform * transform)
		: m_transform(transform)
	{
	}

	Rigidbody::Rigidbody(const Rigidbody & copy)
		: m_transform(copy.m_transform)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}