#include <MemeGraphics/Camera.hpp>

#define ML_CAM_DEFAULT_FOV		45.0f
#define ML_CAM_DEFAULT_PNEAR	0.1f
#define ML_CAM_DEFAULT_PFAR		1000.0f
#define ML_CAM_DEFAULT_ONEAR	-1.0f
#define ML_CAM_DEFAULT_OFAR		+1.0f

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Camera::Camera()
		: m_res(vec2i::Zero)
		, position	(vec3f::Zero)
		, m_target	(vec3f::Forward)
		, m_ortho		(mat4f::Identity())
		, m_persp		(mat4f::Identity())
		, fov		(ML_CAM_DEFAULT_FOV)
		, pNear		(ML_CAM_DEFAULT_PNEAR)
		, pFar		(ML_CAM_DEFAULT_PFAR)
		, oNear		(ML_CAM_DEFAULT_ONEAR)
		, oFar		(ML_CAM_DEFAULT_OFAR)
	{
	}

	Camera::Camera(float fov, float pNear, float pFar, float oNear, float oFar)
		: m_res(vec2i::Zero)
		, position	(vec3f::Zero)
		, m_target	(vec3f::Forward)
		, m_ortho		(mat4f::Identity())
		, m_persp		(mat4f::Identity())
		, fov		(fov)
		, pNear		(pNear)
		, pFar		(pFar)
		, oNear		(oNear)
		, oFar		(oFar)
	{
	}

	Camera::Camera(const Camera & copy)
		: m_res(copy.m_res)
		, position	(copy.position)
		, m_target	(copy.m_target)
		, m_ortho		(copy.m_ortho)
		, m_persp		(copy.m_persp)
		, fov		(copy.fov)
		, pNear		(copy.pNear)
		, pFar		(copy.pFar)
		, oNear		(copy.oNear)
		, oFar		(copy.oFar)
	{
	}

	Camera::~Camera()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Camera & Camera::updateRes(const vec2i & value)
	{
		if ((value != vec2i::Zero) && (this->m_res != value))
		{
			this->m_res = value;

			// Orthographic
			this->m_ortho.update(glm::ortho(
				0.0f,
				0.0f,
				(float)value[0],
				(float)value[1],
				this->oNear, 
				this->oFar
			));

			// Perspective
			this->m_persp.update(glm::perspective(
				this->fov,
				(float)value[0] / (float)value[1],
				this->pNear, 
				this->pFar
			));
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}