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

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLLIDER_HPP_