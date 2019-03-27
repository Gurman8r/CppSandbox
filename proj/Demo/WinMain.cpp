/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>
#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern int32_t main(int32_t argc, char ** argv);

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_SYSTEM_WINDOWS)
int32_t WinMain(void *, void *, char **, int32_t)
{
	return main(__argc, __argv);
}
#endif

/* * * * * * * * * * * * * * * * * * * * */