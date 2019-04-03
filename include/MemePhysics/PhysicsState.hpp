#ifndef _ML_PHYSICS_STATE_HPP_
#define _ML_PHYSICS_STATE_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct PhysicsState final
		: public ITrackable
	{
		List<vec3f> position;
		List<quat> rotation;
		List<mat4f> transform, transformInv;

		PhysicsState();
		PhysicsState(const PhysicsState &copy);
		~PhysicsState();
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_