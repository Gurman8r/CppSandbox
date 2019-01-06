#include <MemeGraphics/Transform.h>

namespace ml
{
	Transform::Transform()
		: m_matrix(mat4f::identity())
	{
	}

	Transform::Transform(const vec3f & position, const vec3f & scale, const quat & rotation)
		: m_matrix()
	{
		this->position(position);
		this->localScale(scale);
		this->rotation(rotation);
	}

	Transform::Transform(const mat4f & value)
		: m_matrix(value)
	{
	}

	Transform::Transform(const Transform & copy)
		: m_matrix(copy.m_matrix)
	{
	}

	Transform::~Transform()
	{
	}


	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		return (*this);
	}

	Transform & Transform::scale(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::translate(const vec3f & value)
	{
		return (*this);
	}
	
	
	Transform & Transform::localScale(const vec3f & value)
	{
		m_matrix[0] = value[0];
		m_matrix[5] = value[1];
		m_matrix[10] = value[2];
		return (*this);
	}
	
	Transform & Transform::position(const vec3f & value)
	{
		m_matrix[12] = value[0];
		m_matrix[13] = value[1];
		m_matrix[14] = value[2];
		return (*this);
	}
	
	Transform & Transform::rotation(const quat & value)
	{
		return (*this);
	}

	Transform & Transform::rotationMatrix(const mat3f & value)
	{
		m_matrix[0] = value[0]; m_matrix[1] = value[1]; m_matrix[2]  = value[2];
		m_matrix[4] = value[3]; m_matrix[5] = value[4]; m_matrix[7]  = value[5];
		m_matrix[8] = value[6]; m_matrix[9] = value[7]; m_matrix[11] = value[8];
		return (*this);
	}


	Transform & Transform::forward(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::backward(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::right(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::left(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::up(const vec3f & value)
	{
		return (*this);
	}

	Transform & Transform::down(const vec3f & value)
	{
		return (*this);
	}
	
	
	const vec3f Transform::localScale() const
	{
		return vec3f(
			m_matrix[0],
			m_matrix[5],
			m_matrix[10]);
	}
	
	const vec3f Transform::position() const
	{
		return vec3f(
			m_matrix[12],
			m_matrix[13],
			m_matrix[14]);
	}
	
	const quat	Transform::rotation() const
	{
		return quat();
	}

	const mat3f Transform::rotationMatrix() const
	{
		return mat3f({
			m_matrix[0], m_matrix[1], m_matrix[2],
			m_matrix[4], m_matrix[5], m_matrix[7],
			m_matrix[8], m_matrix[9], m_matrix[11]
			});
	}


	const vec3f Transform::forward() const
	{
		return vec3f();
	}

	const vec3f Transform::backward() const
	{
		return vec3f();
	}

	const vec3f Transform::right() const
	{
		return vec3f();
	}

	const vec3f Transform::left() const
	{
		return vec3f();
	}

	const vec3f Transform::up() const
	{
		return vec3f();
	}

	const vec3f Transform::down() const
	{
		return vec3f();
	}
	
}