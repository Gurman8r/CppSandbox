#include <MemePhysics/PhysicsState.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState::PhysicsState()
		: m_size(0)
		, m_position()
		, m_rotation()
		, m_transform()
		, m_invTransform()
	{
	}

	PhysicsState::PhysicsState(const PhysicsState & copy)
		: m_size(copy.m_size)
		, m_position(copy.m_position)
		, m_rotation(copy.m_rotation)
		, m_transform(copy.m_transform)
		, m_invTransform(copy.m_invTransform)
	{
	}

	PhysicsState::PhysicsState(PhysicsState && copy)
		: m_size(copy.m_size)
		, m_position(copy.m_position)
		, m_rotation(copy.m_rotation)
		, m_transform(copy.m_transform)
		, m_invTransform(copy.m_invTransform)
	{
	}

	PhysicsState::~PhysicsState()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState & PhysicsState::operator=(const PhysicsState & copy)
	{
		m_size = copy.m_size;
		m_position = copy.m_position;
		m_rotation = copy.m_rotation;
		m_transform = copy.m_transform;
		m_invTransform = copy.m_invTransform;
		return (*this);
	}

	PhysicsState & PhysicsState::operator=(PhysicsState && copy)
	{
		std::swap(m_size, copy.m_size);
		std::swap(m_position, copy.m_position);
		std::swap(m_rotation, copy.m_rotation);
		std::swap(m_transform, copy.m_transform);
		std::swap(m_invTransform, copy.m_invTransform);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	PhysicsState & PhysicsState::resize(const size_t value)
	{
		m_size = value;
		m_position.resize(value);
		m_rotation.resize(value);
		m_transform.resize(value);
		m_invTransform.resize(value);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::getPosition(const size_t index, vec3f & value) const
	{
		if (index < m_position.size())
		{
			value = m_position[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getRotation(const size_t index, quat & value) const
	{
		if (index < m_rotation.size())
		{
			value = m_rotation[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getTransform(const size_t index, mat4f & value) const
	{
		if (index < m_transform.size())
		{
			value = m_transform[index];
			return true;
		}
		value = 0.0f;
		return false;
	}

	bool PhysicsState::getInvTransform(const size_t index, mat4f & value) const
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

	bool PhysicsState::setPosition(const size_t index, const vec3f & value)
	{
		if (index < m_position.size())
		{
			m_position[index] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setRotation(const size_t index, const quat & value)
	{
		if (index < m_rotation.size())
		{
			m_rotation[index] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setTransform(const size_t index, const mat4f & value)
	{
		if (index < m_transform.size())
		{
			m_transform[index] = value;
			return true;
		}
		return false;
	}

	bool PhysicsState::setInvTransform(const size_t index, const mat4f & value)
	{
		if (index < m_invTransform.size())
		{
			m_invTransform[index] = value;
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool PhysicsState::getData(const size_t index, vec3f & pos, quat & rot, mat4f & tf, mat4f & inv) const
	{
		return
			getPosition(index, pos) &&
			getRotation(index, rot) &&
			getTransform(index, tf) &&
			getInvTransform(index, inv);
	}

	bool PhysicsState::setData(const size_t index, const vec3f & pos, const quat & rot, const mat4f & tf, const mat4f & inv)
	{
		return 
			setPosition(index, pos) &&
			setRotation(index, rot) &&
			setTransform(index, tf) &&
			setInvTransform(index, inv);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}