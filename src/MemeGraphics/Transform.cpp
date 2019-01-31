#include <MemeGraphics/Transform.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace ml
{
	Transform::Transform()
		: m_matrix(mat4f::identity())
	{
	}

	Transform::Transform(const vec3f & pos, const vec3f & scl, const quat & rot)
		: m_matrix()
	{
		translate(pos);
		rotate(rot.real(), rot.complex());
		scale(scl);
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


	mat4f Transform::Orthographic(float left, float right, float top, float bottom, float near, float far)
	{
		return mat4f(glm::value_ptr(glm::ortho(left, right, top, bottom, near, far)));
	}

	mat4f Transform::Orthographic(float left, float right, float top, float bottom)
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
				value[0x0], value[0x1], value[0x2], value[0x3],
				value[0x4], value[0x5], value[0x6], value[0x7],
				value[0x8], value[0x9], value[0xA], value[0xB],
				value[0xC], value[0xD], value[0xE], value[0xF]
			),
			angle,
			glm::vec3(axis[0], axis[1], axis[2]))));
	}

	mat4f Transform::Scale(const mat4f & value, const vec3f & vec)
	{
		return mat4f(glm::value_ptr(glm::scale(
			glm::mat4(
				value[0x0],	value[0x1],	value[0x2],	value[0x3],
				value[0x4],	value[0x5],	value[0x6],	value[0x7],
				value[0x8],	value[0x9],	value[0xA],	value[0xB],
				value[0xC],	value[0xD],	value[0xE],	value[0xF]
			),
			glm::vec3(vec[0], vec[1], vec[2]))));
	}

	mat4f Transform::Translate(const mat4f & value, const vec3f & vec)
	{
		return mat4f(glm::value_ptr(glm::translate(
			glm::mat4(
				value[0x0], value[0x1], value[0x2], value[0x3],
				value[0x4], value[0x5], value[0x6], value[0x7],
				value[0x8], value[0x9], value[0xA], value[0xB],
				value[0xC], value[0xD], value[0xE], value[0xF]
			),
			glm::vec3(vec[0], vec[1], vec[2]))));
	}


	Transform & Transform::lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		m_matrix = LookAt(eye, pos, up);
		return (*this);
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
	
}