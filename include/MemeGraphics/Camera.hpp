#ifndef _ML_CAMERA_HPP_
#define _ML_CAMERA_HPP_

#include <MemeGraphics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Camera final
		: public ITrackable
	{
	public:
		Camera();
		Camera(float fieldOfView, float pNear, float pFar, float oNear, float oFar);
		Camera(const Camera & copy);
		~Camera();

	public:
		Camera & update(const vec2i & resolution);

		Camera & lookDir(const vec3f & dir);

		Camera & lookAt(const vec3f & target);

		Camera & orbit(const vec3f & target, const float speed);

	public:
		vec2i		resolution;
		vec3f		position;
		Transform	transform;
		Transform	ortho;
		Transform	persp;
		float		fieldOfView;
		float		perspNear;
		float		perspFar;
		float		orthoNear;
		float		orthoFar;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_