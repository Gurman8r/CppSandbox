#ifndef _ML_I_COMPONENT_HPP_
#define _ML_I_COMPONENT_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_ENGINE_API Component
		: public ITrackable
	{
	public:
		Component()
		{
		}

		virtual ~Component() {}

	};
}

#endif // !_ML_I_COMPONENT_HPP_