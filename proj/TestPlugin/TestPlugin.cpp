/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "TestPlugin.hpp"
#include <MemeCore/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ML_PLUGIN_API void * ML_Plugin_Enable(void * data)
{
	return DEMO::TestPlugin::getInstance().enable(data);
}

ML_PLUGIN_API void * ML_Plugin_Disable(void * data)
{
	return DEMO::TestPlugin::getInstance().disable(data);
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

	void * TestPlugin::enable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return NULL;
	}
	
	void * TestPlugin::disable(void * data)
	{
		ml::cout << ((ml::CString)data) << ml::endl;
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */