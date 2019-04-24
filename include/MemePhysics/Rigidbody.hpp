#ifndef _ML_RIGIDBODY_HPP_
#define _ML_RIGIDBODY_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Rigidbody
		: public ITrackable
	{
	public:
		Rigidbody();
		Rigidbody(int32_t index, Transform * transform);
		Rigidbody(const Rigidbody & copy);
		~Rigidbody();

	public:
		inline const int32_t	index()		const	{ return m_index;		}
		inline const Transform *transform() const	{ return m_transform;	}
		inline Transform *		transform()			{ return m_transform;	}

	private:
		int32_t		m_index;
		Transform * m_transform;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RIGIDBODY_HPP_