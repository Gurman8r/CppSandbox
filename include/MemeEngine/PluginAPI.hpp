#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/Export.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_STRINGIFY(expr) #expr

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PLUGIN_API ML_API_EXPORT

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	class IPlugin;

	using PluginLoadFun = IPlugin * (*)(void);

	using PluginFun	= void * (*)(void *);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_Plugin_Enable	ML_Plugin_Enable
#define ML_Plugin_Disable	ML_Plugin_Disable

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Enable(void * data);
	ML_PLUGIN_API void * ML_Plugin_Disable(void * data);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLUGIN_API_HPP_