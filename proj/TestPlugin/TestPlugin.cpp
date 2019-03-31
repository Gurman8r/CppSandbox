/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Enable(void * data)
{
	return TestPlugin::getInstance().onEnable(data);
}

ML_PLUGIN_API void * ML_Plugin_Disable(void * data)
{
	return TestPlugin::getInstance().onDisable(data);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace
{
	/* * * * * * * * * * * * * * * * * * * * */

	TestPlugin::TestPlugin()
	{
	}

	TestPlugin::~TestPlugin()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * TestPlugin::onEnable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return 0;
	}
	
	void * TestPlugin::onDisable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */