/* * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Vector3.hpp>

ML_API_EXPORT int32_t ML_Plugin_Main(void * user_data)
{
	ml::cout 
		<< "Hello, from Test Plugin!" << ml::endl
		<< ((ml::CString)user_data) << ml::endl
		<< ml::endl;
	
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * */