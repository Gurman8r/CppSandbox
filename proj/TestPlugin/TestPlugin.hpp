#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/PluginAPI.hpp>
#include <MemeCore/ITrackable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Enable(void * data);
	ML_PLUGIN_API void * ML_Plugin_Disable(void * data);
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
		void * onEnable(void * data);
		void * onDisable(void * data);
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_