#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <MemeLib/ITrackable.h>

namespace ml
{
	class ML_API Component 
		: public ITrackable
	{
	public:
		Component() {}
		virtual ~Component() {}

	private:

	};
}

#endif // !_COMPONENT_H_
