#ifndef _ML_PHYSICS_STATE_HPP_
#define _ML_PHYSICS_STATE_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Quaternion.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API PhysicsState final
		: public ITrackable
		, public IDisposable
		, public INonCopyable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		enum Type : uint32_t
		{
			T_Pos,
			T_Rot,
			T_Mat,
			T_Inv,

			MAX_PHYSICS_STATE_TYPE
		};


	public:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		PhysicsState();
		~PhysicsState();


	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		PhysicsState &	deepCopy(const PhysicsState & other);
		bool			dispose() override;
		bool			empty() const;
		int32_t			push();
		PhysicsState &	resize(const size_t value);
		int32_t			size() const;


	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline operator bool() const { return !(this->empty()); }


	public: // Getters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <
			class Elem
		> inline bool get(const int32_t index, const List<Elem> & list, Elem & value) const
		{
			if ((index >= 0) && ((size_t)(index) < list.size()))
			{
				value = list[(size_t)(index)];
				return true;
			}
			return false;
		}

		template <
			Type ID, class Elem = vec3
		> inline bool get(const int32_t index, vec3 & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Pos:
				return this->get<vec3>(index, m_pos, value);
			default:
				return false;
			}
		}

		template <
			Type ID, class Elem = quat
		> inline bool get(const int32_t index, quat & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Rot:
				return this->get<quat>(index, m_rot, value);
			default:
				return false;
			}
		}

		template <
			Type ID, class Elem = mat4
		> inline bool get(const int32_t index, mat4 & value) const
		{
			switch (ID)
			{
			case PhysicsState::T_Mat:
				return this->get<mat4>(index, m_mat, value);
			case PhysicsState::T_Inv:
				return this->get<mat4>(index, m_inv, value);
			default:
				return false;
			}
		}


	public: // Setters
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		template <
			class Elem
		> inline bool set(const int32_t index, List<Elem> & list, const Elem & value)
		{
			if ((index >= 0) && ((size_t)(index) < list.size()))
			{
				list[(size_t)(index)] = value;
				return true;
			}
			return false;
		}

		template <
			Type ID, class Elem = vec3
		> inline bool set(const int32_t index, const vec3 & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Pos: return this->set<vec3>(index, m_pos, value);
			default:
				return false;
			}
		}

		template <
			Type ID, class Elem = quat
		> inline bool set(const int32_t index, const quat & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Rot: return this->set<quat>(index, m_rot, value);
			default: 
				return false;
			}
		}

		template <
			Type ID, class Elem = mat4
		> inline bool set(const int32_t index, const mat4 & value)
		{
			switch (ID)
			{
			case PhysicsState::T_Mat:	return this->set<mat4>(index, m_mat, value);
			case PhysicsState::T_Inv:	return this->set<mat4>(index, m_inv, value);
			default:
				return false;
			}
		}

	private:
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		int32_t		m_size;
		List<vec3>	m_pos;
		List<quat>	m_rot;
		List<mat4>	m_mat;
		List<mat4>	m_inv;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PHYSICS_STATE_HPP_