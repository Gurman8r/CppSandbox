/* * * * * * * * * * * * * * * * * * * * */

#include <MemeCore/Export.hpp>
#include <MemeCore/STD.hpp>

/* * * * * * * * * * * * * * * * * * * * */

extern int32_t main(int32_t argc, char ** argv);

/* * * * * * * * * * * * * * * * * * * * */

#ifdef ML_IMPL_WINDOWS
int32_t WinMain(void *, void *, char **, int32_t)
{ 
	return main(__argc, __argv); 
}
#endif

/* * * * * * * * * * * * * * * * * * * * */