#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

#include <MemeCore/Plugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_API_EXPORT void ML_PluginMain(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_