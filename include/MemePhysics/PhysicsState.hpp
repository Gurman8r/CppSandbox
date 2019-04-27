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
		bool	getPos(const int32_t index, vec3f & pos) const;
		bool	getRot(const int32_t index, quat  & pos) const;
		bool	getMat(const int32_t index, mat4f & pos) const;
		bool	getInv(const int32_t index, mat4f & pos) const;

	public:
		bool	setPos(const int32_t index, const vec3f & value);
		bool	setRot(const int32_t index, const quat  & value);
		bool	setMat(const int32_t index, const mat4f & value);
		bool	setInv(const int32_t index, const mat4f & value);

	private:
		int32_t m_size;

		List<vec3f> m_pos;
		List<quat>	m_rot;
		List<mat4f> m_mat;
		List<mat4f>	m_inv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_