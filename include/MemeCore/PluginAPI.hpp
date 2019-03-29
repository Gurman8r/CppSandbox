#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PLUGIN_API ML_API_EXPORT

#define ML_STRINGIFY(expr) #expr

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	using PluginFun	= void *(*)(void *);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PluginInit		ML_PluginInit
#define ML_PluginEnable		ML_PluginEnable
#define ML_PluginDisable	ML_PluginDisable

#define ML_FUNC_DECL

extern "C"
{
	ML_PLUGIN_API void * ML_PluginInit(void * data);
	ML_PLUGIN_API void * ML_PluginEnable(void * data);
	ML_PLUGIN_API void * ML_PluginDisable(void * data);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLUGIN_API_HPP_