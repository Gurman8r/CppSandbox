#include <MemeCore/LibLoader.h>
#include <MemeCore/FileSystem.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#define ML_INSTANCE static_cast<HINSTANCE>
#else
#define ML_INSTANCE static_cast<void *>
#endif

namespace ml
{
	bool LibLoader::freeLibrary(void * value)
	{
		if (value)
		{
#ifdef ML_SYSTEM_WINDOWS
			return FreeLibrary(ML_INSTANCE(value));
#else
			return false;
#endif
		}
		return false;
	}

	void * LibLoader::loadLibrary(const String & filename)
	{
		if (ML_FileSystem.fileExists(filename))
		{
#ifdef ML_SYSTEM_WINDOWS
			return static_cast<void *>(LoadLibrary(filename.c_str()));
#else
			return NULL;
#endif
		}
		return NULL;
	}

	void * LibLoader::loadFunction(void * value, const String & name)
	{
		if (value)
		{
#ifdef ML_SYSTEM_WINDOWS
			return static_cast<void *>(GetProcAddress(ML_INSTANCE(value), name.c_str()));
#else
			return NULL;
#endif
		}
		return NULL;
	}
}