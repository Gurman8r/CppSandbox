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

	Transform::Transform(const vec3f & pos, const vec3f & scl)
		: m_matrix(mat4f::Identity())
	{
		(*this)
			.translate(pos)
			.rotate(0.f, vec3f::One)
			.scale(scl);
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

	
	// Static Functions
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
		return false;
	}

	mat4f Transform::Rotate(const mat4f & value, const float angle, const vec3f & axis)
	{
		return glm::rotate(
			(glm::mat4)(value), 
			angle, 
			(glm::vec3)(axis)
		);
	}

	mat4f Transform::Rotate(const mat4f & value, const quat & rot)
	{
		return Rotate(
			value,
			rot.angle(),
			rot.axis()
		);
	}

	mat4f Transform::Scale(const mat4f & value, const vec3f & scl)
	{
		return glm::scale((glm::mat4)(value), (glm::vec3)(scl));
	}

	mat4f Transform::Translate(const mat4f & value, const vec3f & tns)
	{
		return glm::translate((glm::mat4)(value), (glm::vec3)(tns));
	}

	mat4f Transform::LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up)
	{
		return glm::lookAt(
			(glm::vec3)(eye),
			(glm::vec3)(pos),
			(glm::vec3)(up)
		);
	}


	// Member Functions
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Transform & Transform::rotate(const float angle, const vec3f & axis)
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

	Transform & Transform::update(const Transform & other)
	{
		return update(other.getMatrix());
	}

	Transform & Transform::update(const mat4f & value)
	{
		m_matrix = value;
		m_changed = true;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Transform::decompose(vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp) const
	{
		return Decompose(m_matrix, scl, rot, tns, skw, psp);
	}

	const Transform & Transform::decompose() const
	{
		if (m_changed)
		{
			if (decompose(m_scl, m_rot, m_pos, m_skw, m_psp))
			{
				m_changed = false;
			}
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const vec4f & Transform::getPsp()	const { return decompose().m_psp; }
	const vec3f & Transform::getPos()	const { return decompose().m_pos; }
	const quat  & Transform::getRot()	const { return decompose().m_rot; }
	const vec3f & Transform::getScl()	const { return decompose().m_scl; }
	const vec3f & Transform::getSkw()	const { return decompose().m_skw; }
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}