#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/PluginAPI.hpp>
#include <MemeCore/ITrackable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_PluginInit(void * data);
	ML_PLUGIN_API void * ML_PluginEnable(void * data);
	ML_PLUGIN_API void * ML_PluginDisable(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace
{
	class ML_PLUGIN_API TestPlugin final
		: public ml::ISingleton<TestPlugin>
	{
		friend class ml::ISingleton<TestPlugin>;

	private:
		TestPlugin();
		~TestPlugin();

	public:
		void * onInit(void * data);
		void * onEnable(void * data);
		void * onDisable(void * data);
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_