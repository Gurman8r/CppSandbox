#include <MemeGraphics/Transform.h>
#include <MemeGraphics/GLM.h>

namespace ml
{
	inline static const glm::mat4 & ToMat4(const mat4f & value)
	{
		static glm::mat4 out;
		return out = glm::mat4(
			value[0],	value[1],	value[2],	value[3],
			value[4],	value[5],	value[6],	value[7],
			value[8],	value[9],	value[10],	value[11],
			value[12],	value[13],	value[14],	value[15]
		);
	}

	inline static const glm::mat3 & ToMat3(const mat4f & value)
	{
		static glm::mat3 out;
		return out = glm::mat3(
			value[0], value[1], value[2],
			value[3], value[4], value[5],
			value[6], value[7], value[8]
		);
	}

	inline static const glm::vec3 & ToVec3(const mat4f & value)
	{
		static glm::vec3 out;
		return out = glm::vec3(value[0], value[1], value[2]);
	}

	inline static const glm::vec2 & ToVec2(const mat4f & value)
	{
		static glm::vec2 out;
		return out = glm::vec2(value[0], value[1]);
	}

	inline static const glm::quat & ToQuat(const mat4f & value)
	{
		static glm::quat out;
		return out = glm::quat(value[0], value[1], value[2], value[3]);
	}

}

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


	mat4f Transform::Ortho(const vec2f & position, const vec2f & size)
	{
		return Ortho(position[0], size[0], position[1], size[1]);
	}

	mat4f Transform::Ortho(float left, float right, float top, float bottom)
	{
		return mat4f(glm::value_ptr(glm::ortho(left, right, top, bottom)));
	}

	mat4f Transform::Persp(float fov, float aspect, float near, float far)
	{
		return mat4f(glm::value_ptr(glm::perspective(fov, aspect, near, far)));
	}


	Transform & Transform::Rotate(float angle, const vec3f & value)
	{
		m_matrix = mat4f(glm::value_ptr(glm::rotate(
			ToMat4(m_matrix),
			(angle * Maths::Deg2Rad),
			glm::vec3(value[0], value[1], value[2]))));
		return changed(true);
	}

	Transform & Transform::Scale(const vec3f & value)
	{
		m_matrix = mat4f(glm::value_ptr(glm::scale(
			ToMat4(m_matrix),
			glm::vec3(value[0], value[1], value[2]))));
		return changed(true);
	}

	Transform & Transform::Translate(const vec3f & value)
	{
		m_matrix = mat4f(glm::value_ptr(glm::translate(
			ToMat4(m_matrix),
			glm::vec3(value[0], value[1], value[2]))));
		return changed(true);
	}
	
	
	Transform & Transform::localScale(const vec3f & value)
	{
		m_matrix[0] = value[0];
		m_matrix[5] = value[1];
		m_matrix[10] = value[2];
		return changed(true);
	}
	
	Transform & Transform::position(const vec3f & value)
	{
		m_matrix[12] = value[0];
		m_matrix[13] = value[1];
		m_matrix[14] = value[2];
		return changed(true);
	}
	
	Transform & Transform::rotation(const quat & value)
	{
		return changed(true);
	}

	Transform & Transform::rotationMatrix(const mat3f & value)
	{
		m_matrix[0] = value[0]; m_matrix[1] = value[1]; m_matrix[2]  = value[2];
		m_matrix[4] = value[3]; m_matrix[5] = value[4]; m_matrix[7]  = value[5];
		m_matrix[8] = value[6]; m_matrix[9] = value[7]; m_matrix[11] = value[8];
		return changed(true);
	}
	
	Transform & Transform::forward(const vec3f & value)
	{
		return changed(true);
	}

	Transform & Transform::backward(const vec3f & value)
	{
		return changed(true);
	}

	Transform & Transform::right(const vec3f & value)
	{
		return changed(true);
	}

	Transform & Transform::left(const vec3f & value)
	{
		return changed(true);
	}

	Transform & Transform::up(const vec3f & value)
	{
		return changed(true);
	}

	Transform & Transform::down(const vec3f & value)
	{
		return changed(true);
	}
	
	
	vec3f Transform::localScale() const
	{
		return vec3f(
			m_matrix[0],
			m_matrix[5],
			m_matrix[10]);
	}
	
	vec3f Transform::position() const
	{
		return vec3f(
			m_matrix[12],
			m_matrix[13],
			m_matrix[14]);
	}
	
	quat Transform::rotation() const
	{
		static glm::quat gq;
		gq = ToQuat(m_matrix);
		return quat(gq[0], gq[1], gq[2], gq[3]);
	}

	mat3f Transform::rotationMatrix() const
	{
		return mat3f({
			m_matrix[0], m_matrix[1], m_matrix[2],
			m_matrix[4], m_matrix[5], m_matrix[7],
			m_matrix[8], m_matrix[9], m_matrix[11]
			});
	}
	
	vec3f Transform::forward() const
	{
		return vec3f();
	}

	vec3f Transform::backward() const
	{
		return vec3f();
	}

	vec3f Transform::right() const
	{
		return vec3f();
	}

	vec3f Transform::left() const
	{
		return vec3f();
	}

	vec3f Transform::up() const
	{
		return vec3f();
	}

	vec3f Transform::down() const
	{
		return vec3f();
	}
	
}