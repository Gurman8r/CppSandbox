#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <MemeGraphics/Export.h>
#include <MemeGraphics/Quaternion.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Vector2.h>

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

		static mat4f Orthographic(float left, float right, float top, float bottom, float near, float far);
		static mat4f Orthographic(float left, float right, float top, float bottom);
		static mat4f Perspective(float fov, float aspect, float near, float far);
		static mat4f LookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		static mat4f Rotate(const mat4f & value, float angle, const vec3f & axis);
		static mat4f Scale(const mat4f & value, const vec3f & vec);
		static mat4f Translate(const mat4f & value, const vec3f & vec);

		Transform & lookAt(const vec3f & eye, const vec3f & pos, const vec3f & up);
		Transform &	rotate(float angle, const vec3f & axis);
		Transform &	scale(const vec3f & value);
		Transform &	translate(const vec3f & value);

		inline const mat4f & matrix() const { return m_matrix; }
		inline operator mat4f() const { return matrix(); }

	private:
		mat4f m_matrix;
	};
}

#endif // !_TRANSFORM_H_
