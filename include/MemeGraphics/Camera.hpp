#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Camera
		: public Transform
	{
	public:
		Camera();
		Camera(const Camera & copy);
		~Camera();

	public:
		void rotateAround(const vec3f & target, const float speed);

	public:
		inline const vec3f & position() const { return m_position; }

	public:
		inline vec3f & position() { return m_position; }

	private:
		vec3f m_position;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_