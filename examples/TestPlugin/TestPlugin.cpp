#include "TestPlugin.hpp"

ML_API_EXPORT int32_t ML_Plugin_Main(void * data)
{
	ml::cout 
		<< "Hello, from Plugin!" << ml::endl
		<< ((ml::CString)data) << ml::endl;

	return EXIT_SUCCESS;
}