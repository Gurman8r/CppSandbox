#include <MemeCore/Transform.hpp>
#include <MemeCore/GLM.hpp>

#define ML_MAT4(value) glm::mat4( \
value[0x0], value[0x1], value[0x2], value[0x3], \
value[0x4], value[0x5], value[0x6], value[0x7], \
value[0x8], value[0x9], value[0xA], value[0xB], \
value[0xC], value[0xD], value[0xE], value[0xF])

#define ML_VEC3(value) glm::vec3(value[0], value[1], value[2])

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

	Transform & Transform::lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return (*this) = mat4f(glm::value_ptr(glm::lookAt(
			ML_VEC3(eye), 
			ML_VEC3(pos), 
			ML_VEC3(up)))
		);
	}

	Transform & Transform::orthographic(const FloatRect & rect)
	{
		return (*this) = mat4f(glm::value_ptr(glm::ortho(
			rect.left(),
			rect.right(),
			rect.top(),
			rect.bot()))
		);
	}

	Transform & Transform::orthographic(const FloatRect & rect, const vec2f & clip)
	{
		return (*this) = mat4f(glm::value_ptr(glm::ortho(
			rect.left(), 
			rect.right(), 
			rect.top(), 
			rect.bot(), 
			clip[0], 
			clip[1]))
		);
	}

	Transform & Transform::perspective(float fov, float aspect, float near, float far)
	{
		return (*this) = mat4f(glm::value_ptr(glm::perspective(
			fov,
			aspect,
			near,
			far))
		);
	}

	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		return (*this) = mat4f(glm::value_ptr(glm::rotate(
			ML_MAT4(m_matrix), 
			angle, 
			ML_VEC3(axis)))
		);
	}

	Transform & Transform::scale(const vec3f & value)
	{
		return (*this) = mat4f(glm::value_ptr(glm::scale(
			ML_MAT4(m_matrix), 
			ML_VEC3(value)))
		);
	}

	Transform & Transform::translate(const vec3f & value)
	{
		return (*this) = mat4f(glm::value_ptr(glm::translate(
			ML_MAT4(m_matrix), 
			ML_VEC3(value)))
		);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Transform::decompose(vec3f & scale, quat & orient, vec3f & trans, vec3f & skew, vec4f & persp)
	{
		static glm::vec3 _scale;
		static glm::quat _orient;
		static glm::vec3 _trans;
		static glm::vec3 _skew;
		static glm::vec4 _persp;

		if (glm::decompose(glm::mat4(
			matrix()[0x0], matrix()[0x1], matrix()[0x2], matrix()[0x3],
			matrix()[0x4], matrix()[0x5], matrix()[0x6], matrix()[0x7],
			matrix()[0x8], matrix()[0x9], matrix()[0xA], matrix()[0xB],
			matrix()[0xC], matrix()[0xD], matrix()[0xE], matrix()[0xF]
		), _scale, _orient, _trans, _skew, _persp))
		{
			scale	= { _scale.x,	_scale.y,	_scale.z				};
			orient	= { _orient.x,	_orient.y,	_orient.z,	_orient.w	};
			trans	= { _trans.x,	_trans.y,	_trans.z				};
			skew	= { _skew.x,	_skew.y,	_skew.z					};
			persp	= { _persp.x,	_persp.y,	_persp.z,	_persp.w	};

			return true;
		}
		else
		{
			scale	= 0.0f;
			orient	= 0.0f;
			trans	= 0.0f;
			skew	= 0.0f;
			persp	= 0.0f;

			return false;
		}
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