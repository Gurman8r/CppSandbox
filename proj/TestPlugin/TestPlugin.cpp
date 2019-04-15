/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Enable(void * data)
{
	return (void *)DEMO::TestPlugin::getInstance().enable(data);
}

ML_PLUGIN_API void * ML_Plugin_Disable(void * data)
{
	return (void *)DEMO::TestPlugin::getInstance().disable(data);
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