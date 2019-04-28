#ifndef _ML_COLLIDER_HPP_
#define _ML_COLLIDER_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_PHYSICS_API Collider
		: public ITrackable
	{
	public:
		Collider();
		virtual ~Collider();

	public:
		virtual bool checkCollision(const Collider & other) const = 0;

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLLIDER_HPP_