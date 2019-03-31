/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Enable(void * data)
{
	return DEMO::TestPlugin::getInstance().onEnable(data);
}

ML_PLUGIN_API void * ML_Plugin_Disable(void * data)
{
	return DEMO::TestPlugin::getInstance().onDisable(data);
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