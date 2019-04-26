#include <MemeCore/Transform.hpp>

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

	Transform::Transform(const vec3f & pos, const vec3f & scl, const quat & rot)
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

	bool Transform::Decompose(const mat4f & value, vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp)
	{
		static glm::vec3 _scl; // scale
		static glm::quat _rot; // orientation
		static glm::vec3 _tns; // translation
		static glm::vec3 _skw; // skew
		static glm::vec4 _psp; // perspective

		if (glm::decompose((glm::mat4)(value), _scl, _rot, _tns, _skw, _psp))
		{
			scl = _scl;
			rot = _rot;
			tns = _tns;
			skw = _skw;
			psp = _psp;
			return true;
		}
		else
		{
			scl = 0.0f;
			rot = 0.0f;
			tns = 0.0f;
			skw = 0.0f;
			psp = 0.0f;
			return false;
		}
	}

	mat4f Transform::Rotate(const mat4f & value, float angle, const vec3f & axis)
	{
		return glm::rotate((glm::mat4)(value), angle, (glm::vec3)(axis));
	}

	mat4f Transform::Rotate(const mat4f & value, const quat & rot)
	{
		return mat4f();
	}

	mat4f Transform::Scale(const mat4f & value, const vec3f & scl)
	{
		return glm::scale((glm::mat4)(value), (glm::vec3)(scl));
	}

	mat4f Transform::Translate(const mat4f & value, const vec3f & trans)
	{
		return glm::translate((glm::mat4)(value), (glm::vec3)(trans));
	}

	mat4f Transform::LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return glm::lookAt(
			(glm::vec3)(eye),
			(glm::vec3)(pos),
			(glm::vec3)(up)
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::rotate(float angle, const vec3f & axis)
	{
		return update(Rotate(m_matrix, angle, axis));
	}

	Transform & Transform::rotate(const quat & value)
	{
		return update(Rotate(m_matrix, value));
	}

	Transform & Transform::scale(const vec3f & value)
	{
		return update(Scale(m_matrix, value));
	}

	Transform & Transform::translate(const vec3f & value)
	{
		return update(Translate(m_matrix, value));
	}

	Transform & Transform::lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return update(LookAt(eye, pos, up));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::update(const mat4f & value)
	{
		m_matrix = value;
		m_changed = true;
		return (*this);
	}

	bool Transform::decompose(vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp) const
	{
		return Decompose(m_matrix, scl, rot, tns, skw, psp);
	}

	void Transform::decompose() const
	{
		if (m_changed)
		{
			if (decompose(m_scl, m_rot, m_pos, m_skw, m_psp))
			{
				m_changed = false;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const vec4f & Transform::getPerspective() const
	{
		decompose(); return m_psp;
	}

	const vec3f & Transform::getPosition() const
	{
		decompose(); return m_pos;
	}
	
	const quat  & Transform::getRotation() const
	{
		decompose(); return m_rot;
	}

	const vec3f & Transform::getScale() const
	{
		decompose(); return m_scl;
	}

	const vec3f & Transform::getSkew() const
	{
		decompose(); return m_skw;
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	Transform & Transform::setPosition(const vec3f & value)
	{
		if (value != getPosition())
		{
			return translate(value - getPosition());
		}
		return (*this);
	}

	Transform & Transform::setRotation(const quat & value)
	{
		// FIXME
		return rotate(value.real(), value.complex());
	}

	Transform & Transform::setScale(const vec3f & value)
	{
		if (value != getScale())
		{
			return scale(value - getScale());
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}