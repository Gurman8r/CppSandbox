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
		clear();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void PhysicsState::clear()
	{
		m_pos.clear();
		m_rot.clear();
		m_mat.clear();
		m_inv.clear();
	}

	bool PhysicsState::empty() const
	{
		return (size() == 0);
	}

	int32_t PhysicsState::push()
	{
		m_size++;
		m_pos.push_back(vec3());
		m_rot.push_back(quat());
		m_mat.push_back(mat4());
		m_inv.push_back(mat4());
		return (size() - 1);
	}

	PhysicsState & PhysicsState::resize(const size_t value)
	{
		if (m_size = (int32_t)(value))
		{
			m_pos.resize(m_size);
			m_rot.resize(m_size);
			m_mat.resize(m_size);
			m_inv.resize(m_size);
		}
		return (*this);
	}

	int32_t PhysicsState::size() const
	{
		return m_size;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::getPos(const int32_t index, vec3 & value) const
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_pos.size()))
		{
			value = m_pos[(size_t)(index)];
			return true;
		}
		return false;
	}

	bool PhysicsState::getRot(const int32_t index, quat & value) const
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_rot.size()))
		{
			value = m_rot[(size_t)(index)];
			return true;
		}
		return false;
	}

	bool PhysicsState::getMat(const int32_t index, mat4 & value) const
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_mat.size()))
		{
			value = m_mat[(size_t)(index)];
			return true;
		}
		return false;
	}

	bool PhysicsState::getInv(const int32_t index, mat4 & value) const
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_inv.size()))
		{
			value = m_inv[(size_t)(index)];
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::setPos(const int32_t index, const vec3 & value)
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_pos.size()))
		{
			m_pos[(size_t)(index)] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setRot(const int32_t index, const quat & value)
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_rot.size()))
		{
			m_rot[(size_t)(index)] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setMat(const int32_t index, const mat4 & value)
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_mat.size()))
		{
			m_mat[(size_t)(index)] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setInv(const int32_t index, const mat4 & value)
	{
		if ((index > PhysicsState::InvalidIndex) && ((size_t)(index) < m_inv.size()))
		{
			m_inv[(size_t)(index)] = value;
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}