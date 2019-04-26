#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <MemeCore/Quaternion.hpp>
#include <MemeCore/Rect.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Transform final
		: public ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform();
		Transform(const vec3f & position, const vec3f & scale);
		Transform(const vec3f & position, const vec3f & scale, const quat & rotation);
		Transform(const mat4f & value);
		Transform(const Transform & copy);
		~Transform();

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		static mat4f Rotate(const mat4f & value, float angle, const vec3f & axis);
		static mat4f Scale(const mat4f & value, const vec3f & scl);
		static mat4f Translate(const mat4f & value, const vec3f & trans);

		static mat4f LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		static mat4f Orthographic(const FloatRect & rect);
		static mat4f Orthographic(const FloatRect & rect, const vec2f & clip);
		static mat4f Perspective(float fov, float aspect, float near, float far);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform &	rotate(float angle, const vec3f & axis);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);
		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Transform & update(const mat4f & value);
		bool decompose(vec3f & scl, quat & rot, vec3f & tns, vec3f & skw, vec4f & psp) const;
		void updateMutables() const;

		const vec3f	& getScale() const;
		const vec3f	& getPosition() const;
		const quat	& getRotation() const;
		const vec3f	& getSkew() const;
		const vec4f	& getPerspective() const;

		Transform & setPosition(const vec3f & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline const mat4f	& matrix() const { return m_matrix; }
		inline mat4f		& matrix()		 { return m_matrix; }

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		mat4f m_matrix;

		mutable bool	m_changed	= true;
		mutable vec3f	m_scl		= 0.0f;
		mutable quat	m_rot		= 1.0f;
		mutable vec3f	m_pos		= 0.0f;
		mutable vec3f	m_skw		= 0.0f;
		mutable vec4f	m_psp		= 0.0f;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_