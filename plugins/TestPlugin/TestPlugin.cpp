/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_PluginInit(void * data)
{
	return TestPlugin::getInstance().onInit(data);
}

ML_PLUGIN_API void * ML_PluginEnable(void * data)
{
	return TestPlugin::getInstance().onEnable(data);
}

ML_PLUGIN_API void * ML_PluginDisable(void * data)
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

	void * TestPlugin::onInit(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return (void *)(NULL);
	}
	
	void * TestPlugin::onEnable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return (void *)(NULL);
	}
	
	void * TestPlugin::onDisable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return (void *)(NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */