#include <MemeCore/Transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define MPX 12	// position X
#define MPY 13	// position Y
#define MPZ 14	// position Z

#define MSX 0	// scale X
#define MSY 5	// scale Y
#define MSZ 10	// scale Z

#define MTX 3	// translation Z
#define MTY 7	// translation Y
#define MTZ 11	// translation Z

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
	value[MSX], value[0x1], value[0x2], value[MTX], \
	value[0x4], value[MSY], value[0x6], value[MTY], \
	value[0x8], value[0x9], value[MSZ], value[MTZ], \
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

	Transform & Transform::invert()
	{
		return ((*this) = this->inverse());
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

	bool Transform::decompose(vec3f & scl, quat & rot, vec3f & pos, vec3f & skw, vec4f & psp)
	{
		static glm::vec3 _scl; // scale
		static glm::quat _rot; // orientation
		static glm::vec3 _pos; // position
		static glm::vec3 _skw; // skew
		static glm::vec4 _psp; // perspective

		if (glm::decompose(ML_MAT4(m_matrix), _scl, _rot, _pos, _skw, _psp))
		{
			scl	= { _scl.x,	_scl.y,	_scl.z			};
			rot	= { _rot.x,	_rot.y,	_rot.z,	_rot.w	};
			pos	= { _pos.x,	_pos.y,	_pos.z			};
			skw	= { _skw.x,	_skw.y,	_skw.z			};
			psp	= { _psp.x,	_psp.y,	_psp.z,	_psp.w	};
			return true;
		}
		else
		{
			scl	= 0.0f;
			rot	= 0.0f;
			pos	= 0.0f;
			skw	= 0.0f;
			psp	= 0.0f;
			return false;
		}
	}

	mat4f Transform::inverse() const
	{
		return mat4f(glm::value_ptr(glm::inverse(ML_MAT4(m_matrix))));
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