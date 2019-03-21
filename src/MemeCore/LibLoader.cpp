#include <MemeCore/LibLoader.hpp>
#include <MemeCore/Macros.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace ml
{
	bool LibLoader::freeLibrary(void * value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return FreeLibrary((HINSTANCE)(value));
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

	void * LibLoader::loadFunction(void * value, const String & name)
	{
#ifdef ML_SYSTEM_WINDOWS
		return GetProcAddress((HINSTANCE)(value), name.c_str());
#else
		return NULL;
#endif
	}

	void * LibLoader::loadPlugin(const String & filename, void * data)
	{
		if (void * lib = loadLibrary(filename))
		{
			if (auto fun = (PluginMainFun)loadFunction(lib, ML_STRINGIFY(ML_Plugin_Main)))
			{
				fun(data);
			}
			return lib;
		}
		return NULL;
	}
}