#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

#include <MemeCore/PluginAPI.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void ML_PluginMain(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_