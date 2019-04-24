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
		PhysicsState(const PhysicsState & copy);
		~PhysicsState();


	public:
		inline const vec3f * getPos(const String & key) const
		{
			HashMap<String, vec3f>::const_iterator it;
			return (((it = m_pos.find(key)) != m_pos.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline const quat  * getRot(const String & key) const
		{
			HashMap<String, quat>::const_iterator it;
			return (((it = m_rot.find(key)) != m_rot.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline const mat4f * getMat(const String & key) const
		{
			HashMap<String, mat4f>::const_iterator it;
			return (((it = m_mat.find(key)) != m_mat.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline const mat4f * getInv(const String & key) const
		{
			HashMap<String, mat4f>::const_iterator it;
			return (((it = m_inv.find(key)) != m_inv.end())
				? (&it->second)
				: (NULL)
			);
		}


	public:
		inline vec3f * getPos(const String & key)
		{
			HashMap<String, vec3f>::iterator it;
			return (((it = m_pos.find(key)) != m_pos.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline quat  * getRot(const String & key)
		{
			HashMap<String, quat>::iterator it;
			return (((it = m_rot.find(key)) != m_rot.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline mat4f * getMat(const String & key)
		{
			HashMap<String, mat4f>::iterator it;
			return (((it = m_mat.find(key)) != m_mat.end())
				? (&it->second)
				: (NULL)
			);
		}

		inline mat4f * getInv(const String & key)
		{
			HashMap<String, mat4f>::iterator it;
			return (((it = m_inv.find(key)) != m_inv.end())
				? (&it->second)
				: (NULL)
			);
		}


	public:
		inline PhysicsState & setPos(const String & key, const vec3f & value)
		{
			(m_pos[key] = value); return (*this);
		}

		inline PhysicsState & setRot(const String & key, const quat	 & value)
		{
			(m_rot[key] = value); return (*this);
		}

		inline PhysicsState & setMat(const String & key, const mat4f & value)
		{
			(m_mat[key] = value); return (*this);
		}

		inline PhysicsState & setInv(const String & key, const mat4f & value)
		{
			(m_inv[key] = value); return (*this);
		}


	private:
		HashMap<String, vec3f>	m_pos;
		HashMap<String, quat>	m_rot;
		HashMap<String, mat4f>	m_mat;
		HashMap<String, mat4f>	m_inv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_