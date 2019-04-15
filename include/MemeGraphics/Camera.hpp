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
		Camera(const vec3f & position, const quat & rotation);
		Camera(const Camera & copy);
		~Camera();

	private:
		vec3f m_position;
		quat m_rotation;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_