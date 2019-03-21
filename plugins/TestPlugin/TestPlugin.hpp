#ifndef _TEST_PLUGIN_HPP_
#define _TEST_PLUGIN_HPP_

#include <MemeCore/Plugin.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern "C"
{
	ML_API_EXPORT int32_t ML_Plugin_Main(void * user_data);
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_TEST_PLUGIN_HPP_