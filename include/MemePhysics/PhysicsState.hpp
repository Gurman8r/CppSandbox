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
		enum : int32_t { InvalidIndex = -1 };

	public:
		PhysicsState();
		PhysicsState(const PhysicsState & copy);
		~PhysicsState();

	public:
		void			clear();
		bool			empty() const;
		PhysicsState &	resize(const size_t value);
		int32_t			size() const;

	public:
		inline operator bool() const
		{
			return !this->empty();
		}

	public:
		bool getPos(const int32_t index, vec3 & pos) const;
		bool getRot(const int32_t index, quat & pos) const;
		bool getMat(const int32_t index, mat4 & pos) const;
		bool getInv(const int32_t index, mat4 & pos) const;

	public:
		bool setPos(const int32_t index, const vec3 & value);
		bool setRot(const int32_t index, const quat & value);
		bool setMat(const int32_t index, const mat4 & value);
		bool setInv(const int32_t index, const mat4 & value);

	private:
		int32_t m_size;

		List<vec3> m_pos;
		List<quat>	m_rot;
		List<mat4> m_mat;
		List<mat4>	m_inv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_