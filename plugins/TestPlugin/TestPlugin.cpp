/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

ML_PLUGIN_API void ML_PluginMain(void * data)
{
	ml::cout << "Hello from Test Plugin!" << ml::endl;
	ml::cout << ((ml::CString)data) << ml::endl;
}

/* * * * * * * * * * * * * * * * * * * * */