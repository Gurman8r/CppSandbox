#include <MemeCore/Transform.hpp>
#include <MemeCore/GLM.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define MPX 0xC // position X
#define MPY 0xD // position Y
#define MPZ 0xE // position Z

#define MSX 0x0 // scale X
#define MSY 0x5 // scale Y
#define MSZ 0xA // scale Z

/* * * * * * * * * * * * * * * * * * * * */

#define ML_VEC2(value) glm::vec2(value[0], value[1])
#define ML_VEC3(value) glm::vec3(value[0], value[1], value[2])
#define ML_VEC4(value) glm::vec4(value[0], value[1], value[2], value[3])
#define ML_QUAT(value) glm::quat(value[0], value[1], value[2], value[3])

#define ML_MAT3(value) glm::mat3( \
	value[0], value[1], value[2], \
	value[3], value[4], value[5], \
	value[6], value[7], value[8])

#define ML_MAT4(value) glm::mat4( \
	value[MSX], value[0x1], value[0x2], value[0x3], \
	value[0x4], value[MSY], value[0x6], value[0x7], \
	value[0x8], value[0x9], value[MSZ], value[0xB], \
	value[MPX], value[MPY], value[MPZ], value[0xF])

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_matrix(mat4f::Identity())
	{
		this->translate(vec3f::Zero);
		this->rotate(0.0f, vec3f::One);
		this->scale(vec3f::One);
	}

	Transform::Transform(const vec3f & position, const vec3f & scale)
		: m_matrix(mat4f::Identity())
	{
		this->translate(position);
		this->rotate(0.f, vec3f::One);
		this->scale(scale);
	}

	Transform::Transform(const vec3f & pos, const vec3f & scl, const quat & rot)
		: m_matrix(mat4f::Identity())
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
			clip.front(), 
			clip.back()))
		);
	}

	Transform & Transform::perspective(const float fov, const float aspect, const float near, const float far)
	{
		return (*this) = mat4f(glm::value_ptr(glm::perspective(
			fov,
			aspect,
			near,
			far))
		);
	}

	Transform & Transform::rotate(const float angle, const vec3f & axis)
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

		if (glm::decompose(ML_MAT4(m_matrix), _scale, _orient, _trans, _skew, _persp))
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

	const vec3f Transform::getPosition() const
	{
		return {
			m_matrix[MPX],
			m_matrix[MPY],
			m_matrix[MPZ]
		};
	}

	const vec3f Transform::getScale() const
	{
		return {
			m_matrix[MSX],
			m_matrix[MSY],
			m_matrix[MSZ]
		};
	}

	const quat Transform::getRotation() const
	{
		return quat();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::setPosition(const vec3f & value)
	{
		m_matrix[MPX] = value[0];
		m_matrix[MPY] = value[1];
		m_matrix[MPZ] = value[2];
		return (*this);
	}

	Transform & Transform::setScale(const vec3f & value)
	{
		m_matrix[MSX] = value[0];
		m_matrix[MSY] = value[1];
		m_matrix[MSZ] = value[2];
		return (*this);
	}

	Transform & Transform::setRotation(const quat & value)
	{
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}