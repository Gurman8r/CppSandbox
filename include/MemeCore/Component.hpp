#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_CORE_API Component 
		: public ITrackable
	{
	public:
		Component() {}
		virtual ~Component() {}

	private:

	};
}

#endif // !_COMPONENT_HPP_
