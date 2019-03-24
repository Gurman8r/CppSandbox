#include <MemeCore/LibLoader.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#else
#
#endif

namespace ml
{
	bool LibLoader::freeLibrary(void * instance)
	{
#ifdef ML_SYSTEM_WINDOWS
		return FreeLibrary((HINSTANCE)(instance));
#else
		return false;
#endif
	}

	void * LibLoader::loadLibrary(const String & filename)
	{
#ifdef ML_SYSTEM_WINDOWS
		return LoadLibraryA(filename.c_str());
#else
		return NULL;
#endif
	}

	void * LibLoader::loadFunction(void * instance, const String & func)
	{
#ifdef ML_SYSTEM_WINDOWS
		return ((instance)
			? GetProcAddress((HINSTANCE)(instance), func.c_str())
			: NULL);
#else
		return NULL;
#endif
	}
}