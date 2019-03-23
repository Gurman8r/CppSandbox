#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

#include <MemeCore/Export.hpp>

#define ML_PLUGIN_API ML_API_EXPORT

#define ML_PluginMain ML_PluginMain

extern "C"
{
	ML_PLUGIN_API void ML_PluginMain(void * data);
}

#endif // !_ML_PLUGIN_API_HPP_
