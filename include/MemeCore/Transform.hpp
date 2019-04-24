#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <MemeCore/Quaternion.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Transform final
		: public ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Transform();
		Transform(const vec3f & position, const vec3f & scale);
		Transform(const vec3f & position, const vec3f & scale, const quat & rotation);
		Transform(const mat4f & value);
		Transform(const Transform & copy);
		~Transform();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const mat4f	& matrix() const { return m_matrix; }
		inline mat4f		& matrix()		 { return m_matrix; }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		Transform & invert();
		Transform & orthographic(const FloatRect & rect);
		Transform & orthographic(const FloatRect & rect, const vec2f & clip);
		Transform & perspective(const float fov, const float aspect, const float near, const float far);
		Transform &	rotate(const float angle, const vec3f & axis);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		bool decompose(vec3f & scl, quat & rot, vec3f & pos);
		bool decompose(vec3f & scl, quat & rot, vec3f & pos, vec3f & skw, vec4f & psp);

		mat4f inverse() const;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		const vec3f getPosition() const;
		const vec3f getScale() const;
		const quat	getRotation() const;

		Transform & setPosition(const vec3f & value);
		Transform & setScale(const vec3f & value);
		Transform & setRotation(const quat & value);

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		mat4f m_matrix;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_