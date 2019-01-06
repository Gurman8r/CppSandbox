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
		~Transform();

		Transform & rotate(float angle, const vec3f & axis);
		Transform & scale(const vec3f & value);
		Transform & translate(const vec3f & value);

		Transform & localScale(const vec3f & value);
		Transform & position(const vec3f & value);
		Transform & rotation(const quat & value);
		Transform & rotationMatrix(const mat3f & value);

		Transform & forward(const vec3f & value);
		Transform & backward(const vec3f & value);
		Transform & right(const vec3f & value);
		Transform & left(const vec3f & value);
		Transform & up(const vec3f & value);
		Transform & down(const vec3f & value);

		const vec3f localScale() const;
		const vec3f position() const;
		const quat	rotation() const;
		const mat3f	rotationMatrix() const;

		const vec3f	forward() const;
		const vec3f backward() const;
		const vec3f right() const;
		const vec3f left() const;
		const vec3f up() const;
		const vec3f down() const;

		inline const mat4f & matrix() const { return m_matrix; }

	private:
		mat4f m_matrix;
	};
}

#endif // !_TRANSFORM_H_
