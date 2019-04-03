#include <MemePhysics/PhysicsState.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState::PhysicsState()
		: m_position()
		, m_rotation()
		, m_transform()
		, m_invTransform()
	{
	}

	PhysicsState::PhysicsState(const PhysicsState & copy)
		: m_position(copy.m_position)
		, m_rotation(copy.m_rotation)
		, m_transform(copy.m_transform)
		, m_invTransform(copy.m_invTransform)
	{
	}

	PhysicsState::~PhysicsState()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState & PhysicsState::resize(const size_t value)
	{
		m_position.resize(value);
		m_rotation.resize(value);
		m_transform.resize(value);
		m_invTransform.resize(value);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::getPosition(const size_t index, vec3f & value)
	{
		if (index < m_position.size())
		{
			value = m_position[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getRotation(const size_t index, quat & value)
	{
		if (index < m_rotation.size())
		{
			value = m_rotation[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getTransform(const size_t index, mat4f & value)
	{
		if (index < m_transform.size())
		{
			value = m_transform[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getInvTransform(const size_t index, mat4f & value)
	{
		if (index < m_invTransform.size())
		{
			value = m_invTransform[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::getData(const size_t index, vec3f & pos, quat & rot, mat4f & tf, mat4f & inv)
	{
		return
			getPosition(index, pos) &&
			getRotation(index, rot) &&
			getTransform(index, tf) &&
			getInvTransform(index, inv);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}