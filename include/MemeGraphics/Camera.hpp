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
		Camera(float fov, float pNear, float pFar, float oNear, float oFar);
		Camera(const Camera & copy);
		~Camera();

	public:
		Camera & updateRes(const vec2i & resolution);
		Camera & lookAt(const vec3f & value);

	public:
		inline vec3f forward() const 
		{
			return (this->target - this->position).normalized(); 
		}
		
		inline vec3f right() const 
		{ 
			return (vec3f::cross(this->forward(), vec3f::Up) * vec3f(1, 0, 1)).normalized(); 
		}

	public:
		vec2i		resolution;
		vec3f		position;
		vec3f		target;
		
		Transform	trans;
		Transform	ortho;
		Transform	persp;

		float		fov;
		float		pNear, pFar;
		float		oNear, oFar;

		vec4f		backgroundColor;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_