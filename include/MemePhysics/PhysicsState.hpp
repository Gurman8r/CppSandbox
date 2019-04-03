#ifndef _ML_PHYSICS_STATE_HPP_
#define _ML_PHYSICS_STATE_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API PhysicsState final
		: public ITrackable
	{
	public:
		PhysicsState();
		PhysicsState(const PhysicsState &copy);
		~PhysicsState();

	public:
		PhysicsState & resize(const size_t value);

	public:
		bool getPosition(const size_t index, vec3f & value) const;
		bool getRotation(const size_t index, quat & value) const;
		bool getTransform(const size_t index, mat4f & value) const;
		bool getInvTransform(const size_t index, mat4f & value) const;

	public:
		bool setPosition(const size_t index, const vec3f & value);
		bool setRotation(const size_t index, const quat & value);
		bool setTransform(const size_t index, const mat4f & value);
		bool setInvTransform(const size_t index, const mat4f & value);

	public:
		bool getData(const size_t index, vec3f & pos, quat & rot, mat4f & tf, mat4f & inv) const;
		bool setData(const size_t index, const vec3f & pos, const quat & rot, const mat4f & tf, const mat4f & inv);

	public:
		inline size_t size() const { return m_size; }

	private:
		size_t m_size;

		List<vec3f> m_position;
		List<quat>	m_rotation;
		List<mat4f> m_transform;
		List<mat4f> m_invTransform;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_