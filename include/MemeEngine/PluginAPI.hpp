#ifndef _ML_PLUGIN_API_HPP_
#define _ML_PLUGIN_API_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/Export.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PLUGIN_API ML_API_EXPORT

#define ML_STRINGIFY(expr) #expr

#define ML_Plugin_Test ML_Plugin_Test


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	using PluginFun	= void * (*)(void *);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Test(void * data);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_PLUGIN_API_HPP_