#ifndef _ML_PLUGIN_HPP_
#define _ML_PLUGIN_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	class ML_CORE_API Plugin
		: public ITrackable
	{
	public:
		Plugin();
		~Plugin();

	private:

	};
}

#endif // !_ML_PLUGIN_HPP_
