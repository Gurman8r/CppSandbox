#ifndef _ML_AABB_HPP_
#define _ML_AABB_HPP_

#include <MemePhysics/Export.hpp>
#include <MemePhysics/Collider.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API AABB
		: public Collider
	{
	public:
		AABB();
		~AABB();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AABB_HPP_