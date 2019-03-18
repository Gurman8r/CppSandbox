/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Config.hpp>
#include <stdlib.h>
#include <stdint.h>

/* * * * * * * * * * * * * * * * * * * * */

extern int32_t ml_main(int32_t argc, char ** argv);

/* * * * * * * * * * * * * * * * * * * * */

# if !defined(ML_DEBUG) && defined(ML_SYSTEM_WINDOWS)
int32_t WinMain(void *, void *, char **, int32_t)
# else
int32_t main(int32_t, char **)
# endif
{
	return ml_main(__argc, __argv);
}

/* * * * * * * * * * * * * * * * * * * * */