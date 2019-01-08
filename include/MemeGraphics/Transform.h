#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Quaternion.h>

namespace ml
{
	class ML_GRAPHICS_API Transform final
		: public ITrackable
	{
	public:
		Transform();
		Transform(const vec3f & position, const vec3f & scale, const quat & rotation);
		Transform(const mat4f & value);
		Transform(const Transform & copy);
		~Transform();

		static mat4f Ortho(const vec2f & position, const vec2f & size);
		static mat4f Ortho(float left, float right, float top, float bottom);
		static mat4f Persp(float fov, float aspect, float near, float far);

		Transform &	Rotate(float angle, const vec3f & axis);
		Transform &	Scale(const vec3f & value);
		Transform &	Translate(const vec3f & value);

		Transform &	localScale(const vec3f & value);
		Transform &	position(const vec3f & value);
		Transform &	rotation(const quat & value);
		Transform &	rotationMatrix(const mat3f & value);

		Transform &	forward(const vec3f & value);
		Transform &	backward(const vec3f & value);
		Transform &	right(const vec3f & value);
		Transform &	left(const vec3f & value);
		Transform &	up(const vec3f & value);
		Transform &	down(const vec3f & value);

		vec3f	localScale() const;
		vec3f	position() const;
		quat	rotation() const;
		mat3f	rotationMatrix() const;

		vec3f	forward() const;
		vec3f	backward() const;
		vec3f	right() const;
		vec3f	left() const;
		vec3f	up() const;
		vec3f	down() const;

		inline const mat4f & matrix() const { return m_matrix; }

		inline operator mat4f() const { return matrix(); }

	private:
		mat4f m_matrix;

		Transform & changed(bool value) { m_changed = value; return (*this); }
		
		mutable bool m_changed;
	};
}

#endif // !_TRANSFORM_H_
