#ifndef _ML_BOX_COLLIDER_HPP_
#define _ML_BOX_COLLIDER_HPP_

#include <MemePhysics/Collider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API BoxCollider
		: public Collider
	{
	public:
		BoxCollider();
		BoxCollider(const vec3 & size);
		BoxCollider(const BoxCollider & copy);
		~BoxCollider();

	public:
		inline const vec3 & size() const	{ return m_size; }
		inline vec3 &		size()			{ return m_size; }

	private:
		vec3 m_size;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BOX_COLLIDER_HPP_