#ifndef _RIGIDBODY_HPP_
#define _RIGIDBODY_HPP_

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

#endif // !_RIGIDBODY_HPP_
