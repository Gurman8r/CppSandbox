#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <MemeGraphics/Transform.hpp>

namespace ml
{
	class ML_GRAPHICS_API Camera
		: public Transform
	{
	public:
		Camera();
		Camera(const vec3f & position, const vec3f & scale, const quat & rotation);
		Camera(const mat4f & value);
		Camera(const Camera & copy);
		~Camera();

	private:

	};
}

#endif // !_CAMERA_HPP_
