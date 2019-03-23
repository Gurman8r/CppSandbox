#ifndef _ML_I_COMPONENT_HPP_
#define _ML_I_COMPONENT_HPP_

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

#endif // !_ML_I_COMPONENT_HPP_