#ifndef _COMPONENT_H_
#define _COMPONENT_H_

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

#endif // !_COMPONENT_H_
