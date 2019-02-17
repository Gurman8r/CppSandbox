#include <MemeGraphics/Transform.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_matrix(mat4f::identity())
	{
		this->translate(vec3f::Zero);
		this->rotate(0.0f, vec3f::One);
		this->scale(vec3f::One);
	}

	Transform::Transform(const vec3f & position, const vec3f & scale)
		: m_matrix(mat4f::identity())
	{
		this->translate(position);
		this->rotate(0.f, vec3f::One);
		this->scale(scale);
	}

	Transform::Transform(const vec3f & pos, const vec3f & scl, const quat & rot)
		: m_matrix(mat4f::identity())
	{
		this->translate(pos);
		this->rotate(rot.real(), rot.complex());
		this->scale(scl);
	}

	Transform::Transform(const mat4f & value)
		: m_matrix(value)
	{
	}

	Transform::Transform(const Transform & copy)
		: Transform(copy.m_matrix)
	{
	}

	Transform::~Transform()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	mat4f Transform::Orthographic(const FloatRect & rect, const vec2f & clip)
	{
		return Orthographic(rect, clip[0], clip[1]);
	}

	mat4f Transform::Orthographic(const FloatRect & rect, float N, float F)
	{
		return Orthographic(rect.left(), rect.right(), rect.top(), rect.bot(), N, F);
	}

	mat4f Transform::Orthographic(const FloatRect & rect)
	{
		return Orthographic(rect.left(), rect.right(), rect.top(), rect.bot());
	}

	mat4f Transform::Orthographic(float L, float R, float T, float B, float N, float F)
	{
		return mat4f(glm::value_ptr(glm::ortho(L, R, T, B, N, F)));
	}

	mat4f Transform::Orthographic(float L, float R, float T, float B)
	{
		return mat4f(glm::value_ptr(glm::ortho(L, R, T, B)));
	}

	mat4f Transform::Perspective(float fov, float aspect, const vec2f & clip)
	{
		return Perspective(fov, aspect, clip[0], clip[1]);
	}

	mat4f Transform::Perspective(float fov, float aspect, float N, float F)
	{
		return mat4f(glm::value_ptr(glm::perspective(fov, aspect, N, F)));
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

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return (*this) = LookAt(eye, pos, up);
	}

	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		return (*this) = Rotate((*this), angle, axis);
	}

	Transform & Transform::scale(const vec3f & value)
	{
		return (*this) = Scale((*this), value);
	}

	Transform & Transform::translate(const vec3f & value)
	{
		return (*this) = Translate((*this), value);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const vec3f & Transform::getPosition() const
	{
		static vec3f temp;
		temp[0] = matrix()[0xC];
		temp[1] = matrix()[0xD];
		temp[2] = matrix()[0xE];
		return temp;
	}

	const vec3f & Transform::getScale() const
	{
		static vec3f temp;
		temp[0] = matrix()[0x0];
		temp[1] = matrix()[0x5];
		temp[2] = matrix()[0xA];
		return temp;
	}

	const quat & Transform::getRotation() const
	{
		static quat temp;
		return temp;
	}


	Transform & Transform::setPosition(const vec3f & value)
	{
		matrix()[0xC] = value[0];
		matrix()[0xD] = value[1];
		matrix()[0xE] = value[2];
		return (*this);
	}

	Transform & Transform::setScale(const vec3f & value)
	{
		matrix()[0x0] = value[0];
		matrix()[0x5] = value[1];
		matrix()[0xA] = value[2];
		return (*this);
	}

	Transform & Transform::setRotation(const quat & value)
	{
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}