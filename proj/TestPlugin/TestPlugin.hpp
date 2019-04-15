#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <MemeEngine/IPlugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_PLUGIN_API void * ML_Plugin_Enable(void * data);
	ML_PLUGIN_API void * ML_Plugin_Disable(void * data);
}

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	class ML_PLUGIN_API TestPlugin final
		: public ml::IPlugin
		, public ml::ISingleton<TestPlugin>
	{
		friend class ml::ISingleton<TestPlugin>;

	private:
		TestPlugin();
		~TestPlugin();

	public:
		int32_t enable(void * data) override;
		int32_t disable(void * data) override;
	};
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_