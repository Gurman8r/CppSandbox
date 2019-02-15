#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeGraphics/Quaternion.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Rect.hpp>

namespace ml
{
	class ML_GRAPHICS_API Transform
		: public ITrackable
	{
	public:
		Transform();
		Transform(const vec3f & position, const vec3f & scale);
		Transform(const vec3f & position, const vec3f & scale, const quat & rotation);
		Transform(const mat4f & value);
		Transform(const Transform & copy);
		~Transform();

		static mat4f Orthographic(const FloatRect & rect, const vec2f & clip);
		static mat4f Orthographic(const FloatRect & rect, float N, float F);
		static mat4f Orthographic(const FloatRect & rect);
		static mat4f Orthographic(float L, float R, float T, float B, float N, float F);
		static mat4f Orthographic(float L, float R, float T, float B);
		static mat4f Perspective(float fov, float aspect, const vec2f & clip);
		static mat4f Perspective(float fov, float aspect, float N, float F);
		static mat4f LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		static mat4f Rotate(const mat4f & value, float angle, const vec3f & axis);
		static mat4f Scale(const mat4f & value, const vec3f & vec);
		static mat4f Translate(const mat4f & value, const vec3f & vec);

		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		Transform &	rotate(float angle, const vec3f & axis);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);

	public:
		inline operator	const mat4f &()	const { return matrix(); }
		inline const mat4f & matrix()	const { return m_matrix; }

		inline operator mat4f &() { return matrix(); }
		inline mat4f &	matrix()  { return m_matrix; }

	public:
		const vec3f & getPosition() const;
		const vec3f & getScale() const;
		const quat & getRotation() const;

		Transform & setPosition(const vec3f & value);
		Transform & setScale(const vec3f & value);
		Transform & setRotation(const quat & value);

	private:
		mat4f m_matrix;
	};
}

#endif // !_TRANSFORM_HPP_
