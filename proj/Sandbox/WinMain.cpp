/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#if defined(ML_SYSTEM_WINDOWS)

#	include <Windows.h>
#	include <MemeCore/STD.hpp>

extern int32_t main(int32_t, char **);

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}

#endif

/* * * * * * * * * * * * * * * * * * * * */