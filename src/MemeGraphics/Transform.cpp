#include <MemeGraphics/Transform.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

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


	mat4f Transform::Ortho(float left, float right, float top, float bottom, float near, float far)
	{
		return mat4f(glm::value_ptr(glm::ortho(left, right, top, bottom, near, far)));
	}

	mat4f Transform::Ortho(float left, float right, float top, float bottom)
	{
		return mat4f(glm::value_ptr(glm::ortho(left, right, top, bottom)));
	}

	mat4f Transform::Perspective(float fov, float aspect, float near, float far)
	{
		return mat4f(glm::value_ptr(glm::perspective(fov, aspect, near, far)));
	}


	mat4f Transform::LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return mat4f(glm::value_ptr(glm::lookAt(
			glm::vec3(eye[0], eye[1], eye[2]),
			glm::vec3(pos[0], pos[1], pos[2]),
			glm::vec3( up[0],  up[1],  up[2]))));
	}


	mat4f Transform::Rotate(const mat4f & value, float angle, const vec3f & axis)
	{
		return mat4f(glm::value_ptr(glm::rotate(
			glm::mat4(
				value[ 0],	value[ 1],	value[ 2],	value[ 3],
				value[ 4],	value[ 5],	value[ 6],	value[ 7],
				value[ 8],	value[ 9],	value[10],	value[11],
				value[12],	value[13],	value[14],	value[15]
			),
			angle,
			glm::vec3(axis[0], axis[1], axis[2]))));
	}

	mat4f Transform::Scale(const mat4f & value, const vec3f & vec)
	{
		return mat4f(glm::value_ptr(glm::scale(
			glm::mat4(
				value[ 0],	value[ 1],	value[ 2],	value[ 3],
				value[ 4],	value[ 5],	value[ 6],	value[ 7],
				value[ 8],	value[ 9],	value[10],	value[11],
				value[12],	value[13],	value[14],	value[15]
			),
			glm::vec3(vec[0], vec[1], vec[2]))));
	}

	mat4f Transform::Translate(const mat4f & value, const vec3f & vec)
	{
		return mat4f(glm::value_ptr(glm::translate(
			glm::mat4(
				value[ 0],	value[ 1],	value[ 2],	value[ 3],
				value[ 4],	value[ 5],	value[ 6],	value[ 7],
				value[ 8],	value[ 9],	value[10],	value[11],
				value[12],	value[13],	value[14],	value[15]
			),
			glm::vec3(vec[0], vec[1], vec[2]))));
	}


	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		m_matrix = Rotate(m_matrix, angle, axis);
		return (*this);
	}

	Transform & Transform::scale(const vec3f & value)
	{
		m_matrix = Scale(m_matrix, value);
		return (*this);
	}

	Transform & Transform::translate(const vec3f & value)
	{
		m_matrix = Translate(m_matrix, value);
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
	
	
	const vec3f & Transform::localScale() const
	{
		static vec3f temp;
		temp[0] = m_matrix[0];
		temp[1] = m_matrix[5];
		temp[2] = m_matrix[10];
		return temp;
	}
	
	const vec3f & Transform::position() const
	{
		static vec3f temp;
		temp[0] = m_matrix[12];
		temp[1] = m_matrix[13];
		temp[2] = m_matrix[14];
		return temp;
	}
	
	const quat & Transform::rotation() const
	{
		static quat temp;
		return temp;
	}

	const mat3f & Transform::rotationMatrix() const
	{
		static mat3f temp;
		temp[0] = m_matrix[0]; temp[1] = m_matrix[1]; temp[2] = m_matrix[2];
		temp[4] = m_matrix[3]; temp[5] = m_matrix[4]; temp[7] = m_matrix[5];
		temp[8] = m_matrix[6]; temp[9] = m_matrix[7]; temp[11] = m_matrix[8];
		return temp;
	}
	
	const vec3f & Transform::forward() const
	{
		static vec3f temp;
		return temp;
	}

	const vec3f & Transform::backward() const
	{
		static vec3f temp;
		return temp;
	}

	const vec3f & Transform::right() const
	{
		static vec3f temp;
		return temp;
	}

	const vec3f & Transform::left() const
	{
		static vec3f temp;
		return temp;
	}

	const vec3f & Transform::up() const
	{
		static vec3f temp;
		return temp;
	}

	const vec3f & Transform::down() const
	{
		static vec3f temp;
		return temp;
	}
	
}