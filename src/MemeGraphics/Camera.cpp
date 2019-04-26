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
		: resolution(vec2i::Zero)
		, position	(vec3f::Zero)
		, target	(vec3f::Forward)
		, trans		(mat4f::Identity())
		, ortho		(mat4f::Identity())
		, persp		(mat4f::Identity())
		, fov		(ML_CAM_DEFAULT_FOV)
		, pNear		(ML_CAM_DEFAULT_PNEAR)
		, pFar		(ML_CAM_DEFAULT_PFAR)
		, oNear		(ML_CAM_DEFAULT_ONEAR)
		, oFar		(ML_CAM_DEFAULT_OFAR)
	{
	}

	Camera::Camera(float fov, float pNear, float pFar, float oNear, float oFar)
		: resolution(vec2i::Zero)
		, position	(vec3f::Zero)
		, target	(vec3f::Forward)
		, trans		(mat4f::Identity())
		, ortho		(mat4f::Identity())
		, persp		(mat4f::Identity())
		, fov		(fov)
		, pNear		(pNear)
		, pFar		(pFar)
		, oNear		(oNear)
		, oFar		(oFar)
	{
	}

	Camera::Camera(const Camera & copy)
		: resolution(copy.resolution)
		, position	(copy.position)
		, target	(copy.target)
		, trans		(copy.trans)
		, ortho		(copy.ortho)
		, persp		(copy.persp)
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

	Camera & Camera::update(const vec2i & value)
	{
		if ((value != vec2i::Zero) && (this->resolution != value))
		{
			this->resolution = value;

			// Orthographic
			this->ortho.update(glm::ortho(
				0.0f,
				0.0f,
				(float)value[0],
				(float)value[1],
				this->oNear, 
				this->oFar
			));

			// Perspective
			this->persp.update(glm::perspective(
				this->fov,
				(float)value[0] / (float)value[1],
				this->pNear, 
				this->pFar
			));
		}
		return (*this);
	}

	Camera & Camera::lookAt(const vec3f & value)
	{
		this->trans.lookAt(
			this->position,
			(this->target = (this->position + (value - this->position).normalized())),
			vec3f::Up
		);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}