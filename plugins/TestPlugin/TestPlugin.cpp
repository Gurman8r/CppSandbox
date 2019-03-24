/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void ML_PluginInit(void * data)
{
	TestPlugin::getInstance().onInit(data);
}

ML_PLUGIN_API void ML_PluginEnable(void * data)
{
	TestPlugin::getInstance().onEnable(data);
}

ML_PLUGIN_API void ML_PluginDisable(void * data)
{
	TestPlugin::getInstance().onDisable(data);
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

	void TestPlugin::onInit(void * data)
	{
		ml::Debug::log("Hello From Plugin!");
		ml::cout << ((ml::CString)data) << ml::endl;
	}
	
	void TestPlugin::onEnable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
	}
	
	void TestPlugin::onDisable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */