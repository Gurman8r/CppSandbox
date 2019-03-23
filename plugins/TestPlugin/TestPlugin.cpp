/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

ML_API_EXPORT void ML_PluginMain(void * user_data)
{
	ml::cout << "Hello from Test Plugin!" << ml::endl;
	ml::cout << ((ml::CString)user_data) << ml::endl;
}

/* * * * * * * * * * * * * * * * * * * * */