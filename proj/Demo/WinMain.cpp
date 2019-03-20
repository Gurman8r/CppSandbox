/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>
#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern int32_t main(int32_t argc, char ** argv);

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_SYSTEM_WINDOWS

extern "C" ML_API_EXPORT unsigned long NvOptimusEnablement = 1;
extern "C" ML_API_EXPORT unsigned long AmdPowerXpressRequestHighPerformance = 1;

int32_t WinMain(void *, void *, char **, int32_t)
{
	return main(__argc, __argv);
}

#endif

/* * * * * * * * * * * * * * * * * * * * */