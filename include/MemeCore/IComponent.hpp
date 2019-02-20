#ifndef _I_COMPONENT_HPP_
#define _I_COMPONENT_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_CORE_API IComponent 
		: public ITrackable
	{
	public:
		IComponent() {}
		virtual ~IComponent() {}

	private:

	};
}

#endif // !_COMPONENT_HPP_
