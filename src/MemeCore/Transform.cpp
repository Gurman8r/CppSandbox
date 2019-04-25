#include <MemeCore/Transform.hpp>
#include <MemeCore/GLM.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform::Transform()
		: m_matrix(mat4f::Identity())
	{
		(*this)
			.translate(vec3f::Zero)
			.rotate(0.0f, vec3f::One)
			.scale(vec3f::One);
	}

	Transform::Transform(const vec3f & position, const vec3f & scale)
		: m_matrix(mat4f::Identity())
	{
		(*this)
			.translate(position)
			.rotate(0.f, vec3f::One)
			.scale(scale);
	}

	Transform::Transform(const vec3f & pos, const vec3f & scl, const Quat & rot)
		: m_matrix(mat4f::Identity())
	{
		(*this)
			.translate(pos)
			.rotate(rot.real(), rot.complex())
			.scale(scl);
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Transform::Decompose(const mat4f & value, vec3f & scl, Quat & rot, vec3f & tns, vec3f & skw, vec4f & psp)
	{
		//glm::project()
		static glm::vec3 _scl; // scale
		static glm::quat _rot; // orientation
		static glm::vec3 _tns; // translation
		static glm::vec3 _skw; // skew
		static glm::vec4 _psp; // perspective

		if (glm::decompose(ML_MAT4(value), _scl, _rot, _tns, _skw, _psp))
		{
			scl	= { _scl.x,	_scl.y,	_scl.z			};
			rot	= { _rot.x,	_rot.y,	_rot.z,	_rot.w	};
			tns	= { _tns.x,	_tns.y,	_tns.z			};
			skw	= { _skw.x,	_skw.y,	_skw.z			};
			psp	= { _psp.x,	_psp.y,	_psp.z,	_psp.w	};
			return true;
		}
		else
		{
			scl	= 0.0f;
			rot	= 0.0f;
			tns	= 0.0f;
			skw	= 0.0f;
			psp	= 0.0f;
			return false;
		}
	}

	mat4f Transform::Inverse(const mat4f & value)
	{
		return mat4f(glm::value_ptr(glm::inverse(ML_MAT4(value))));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	mat4f Transform::Rotate(const mat4f & value, float angle, const vec3f & axis)
	{
		return mat4f(glm::value_ptr(glm::rotate(ML_MAT4(value), angle, ML_VEC3(axis))));
	}

	mat4f Transform::Scale(const mat4f & value, const vec3f & scl)
	{
		return mat4f(glm::value_ptr(glm::scale(ML_MAT4(value), ML_VEC3(scl))));
	}

	mat4f Transform::Translate(const mat4f & value, const vec3f & trans)
	{
		return mat4f(glm::value_ptr(glm::translate(ML_MAT4(value), ML_VEC3(trans))));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	mat4f Transform::LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return mat4f(glm::value_ptr(glm::lookAt(
			ML_VEC3(eye),
			ML_VEC3(pos),
			ML_VEC3(up)))
		);
	}

	mat4f Transform::Orthographic(const FloatRect & rect)
	{
		return mat4f(glm::value_ptr(glm::ortho(
			rect.left(),
			rect.right(),
			rect.top(),
			rect.bot()))
		);
	}

	mat4f Transform::Orthographic(const FloatRect & rect, const vec2f & clip)
	{
		return mat4f(glm::value_ptr(glm::ortho(
			rect.left(),
			rect.right(),
			rect.top(),
			rect.bot(),
			clip.front(),
			clip.back()))
		);
	}

	mat4f Transform::Perspective(float fov, float aspect, float near, float far)
	{
		return mat4f(glm::value_ptr(glm::perspective(
			fov,
			aspect,
			near,
			far))
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::invert()
	{
		return ((*this) = Transform::Inverse(m_matrix));
	}

	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		return ((*this) = Transform::Rotate(m_matrix, angle, axis));
	}

	Transform & Transform::scale(const vec3f & value)
	{
		return ((*this) = Transform::Scale(m_matrix, value));
	}

	Transform & Transform::translate(const vec3f & value)
	{
		return ((*this) = Transform::Translate(m_matrix, value));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Transform::decompose(vec3f & scl, Quat & rot, vec3f & tns, vec3f & skw, vec4f & psp) const
	{
		return Transform::Decompose(m_matrix, scl, rot, tns, skw, psp);
	}
	
	mat4f Transform::getInverse() const
	{
		return Transform::Inverse(m_matrix);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	const vec3f Transform::getPosition() const
	{
		return {
			m_matrix[12],
			m_matrix[13],
			m_matrix[14]
		};
	}
	
	Transform & Transform::setPosition(const vec3f & value)
	{
		m_matrix[12] = value[0];
		m_matrix[13] = value[1];
		m_matrix[14] = value[2];
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}