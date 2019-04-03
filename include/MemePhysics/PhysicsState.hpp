#ifndef _ML_PHYSICS_STATE_HPP_
#define _ML_PHYSICS_STATE_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class PhysicsState final
		: public ITrackable
	{
	public:
		PhysicsState();
		PhysicsState(const PhysicsState &copy);
		~PhysicsState();

	public:
		PhysicsState & resize(const size_t value);

	public:
		bool getPosition(const size_t index, vec3f & value);
		bool getRotation(const size_t index, quat & value);
		bool getTransform(const size_t index, mat4f & value);
		bool getInvTransform(const size_t index, mat4f & value);

	public:
		bool getData(const size_t index, vec3f & pos, quat & rot, mat4f & tf, mat4f & inv);

	private:
		List<vec3f> m_position;
		List<quat>	m_rotation;
		List<mat4f> m_transform;
		List<mat4f> m_invTransform;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_