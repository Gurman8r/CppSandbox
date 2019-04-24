#include <MemePhysics/PhysicsState.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState::PhysicsState()
		: m_pos()
		, m_rot()
		, m_mat()
		, m_inv()
	{
	}

	PhysicsState::PhysicsState(const PhysicsState & copy)
		: m_pos(copy.m_pos)
		, m_rot(copy.m_rot)
		, m_mat(copy.m_mat)
		, m_inv(copy.m_inv)
	{
	}

	PhysicsState::~PhysicsState()
	{
		m_pos.clear();
		m_rot.clear();
		m_mat.clear();
		m_inv.clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	/* * * * * * * * * * * * * * * * * * * * */
}