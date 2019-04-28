#ifndef _ML_SPHERE_COLLIDER_HPP_
#define _ML_SPHERE_COLLIDER_HPP_

#include <MemePhysics/Collider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API SphereCollider final
		: public Collider
	{
	public:
		SphereCollider();
		SphereCollider(const float radius);
		SphereCollider(const SphereCollider & copy);
		~SphereCollider();

	public:
		bool checkCollision(const Collider & other) const override;

	public:
		inline const float	radius() const	{ return m_radius; }
		inline float &		radius()		{ return m_radius; }

	private:
		float m_radius;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPHERE_COLLIDER_HPP_