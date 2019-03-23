#ifndef _ML_RIGIDBODY_HPP_
#define _ML_RIGIDBODY_HPP_

#include <MemePhysics/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_PHYSICS_API Rigidbody
		: public ITrackable
	{
	public:
		Rigidbody();
		~Rigidbody();

	private:

	};
}

#endif // !_ML_RIGIDBODY_HPP_