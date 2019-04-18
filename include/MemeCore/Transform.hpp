#ifndef _ML_TRANSFORM_HPP_
#define _ML_TRANSFORM_HPP_

#include <MemeCore/Quaternion.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Rect.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Transform
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
		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		Transform & orthographic(const FloatRect & rect);
		Transform & orthographic(const FloatRect & rect, const vec2f & clip);
		Transform & perspective(float fov, float aspect, float near, float far);
		Transform &	rotate(float angle, const vec3f & axis);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);

		bool decompose(vec3f & scale, quat  & orient, vec3f & trans, vec3f & skew, vec4f & persp);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		const vec3f getPosition() const;
		const vec3f getScale() const;
		const quat	getRotation() const;

		Transform & setPosition(const vec3f & value);
		Transform & setScale(const vec3f & value);
		Transform & setRotation(const quat & value);

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline const mat4f	& matrix() const { return m_matrix; }
		inline mat4f		& matrix()		 { return m_matrix; }

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		mat4f m_matrix;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_TRANSFORM_HPP_