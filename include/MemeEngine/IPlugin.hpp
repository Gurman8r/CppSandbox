#ifndef _ML_I_PLUGIN_HPP_
#define _ML_I_PLUGIN_HPP_

#include <MemeEngine/PluginAPI.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API IPlugin
	{
	public:
		virtual ~IPlugin() {}

		virtual int32_t enable(void *) = 0;
		virtual int32_t disable(void *) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_PLUGIN_HPP_