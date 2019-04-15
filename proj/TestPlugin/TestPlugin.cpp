/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Enable(void * data)
{
	DEMO::TestPlugin::getInstance().enable(data);
	return NULL;
}

ML_PLUGIN_API void * ML_Plugin_Disable(void * data)
{
	DEMO::TestPlugin::getInstance().disable(data);
	return NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	/* * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin()
	{
	}

	TestPlugin::~TestPlugin()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	int32_t TestPlugin::enable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return 0;
	}
	
	int32_t TestPlugin::disable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */