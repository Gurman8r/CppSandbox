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

	public:
		inline const Transform &	getOrtho() const		{ return m_ortho; }
		inline const Transform &	getPersp() const		{ return m_persp; }
		inline const mat4 &		getOrthoMatrix() const	{ return m_ortho.getMatrix(); }
		inline const mat4 &		getPerspMatrix() const	{ return m_persp.getMatrix(); }

		inline const vec3 & forward(const vec3 & value)
		{
			return (m_target = this->position + value.normalized());
		}

		inline vec3 forward() const 
		{
			return (m_target - this->position).normalized();
		}
		
		inline vec3 right() const 
		{ 
			return (forward().cross(vec3::Up) * vec3(1, 0, 1)).normalized();
		}

	public:
		vec3		position;
		float		fov;
		float		pNear, pFar;
		float		oNear, oFar;
		vec4		backgroundColor;

	private:
		vec2i		m_res;
		vec3		m_target;
		Transform	m_ortho;
		Transform	m_persp;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CAMERA_HPP_