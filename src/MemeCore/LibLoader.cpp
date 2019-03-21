#include <MemeCore/LibLoader.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	define ML_INSTANCE(expr) ((HINSTANCE)(expr))
#	define ML_FreeLibrary FreeLibrary
#	define ML_LoadLibrary LoadLibrary
#	define ML_LoadFunction GetProcAddress
#else
#	define ML_INSTANCE(expr) ((void *)(expr))
#	define ML_FreeLibrary(expr)
#	define ML_LoadLibrary(expr) 
#	define ML_LoadFunction(expr)
#endif

namespace ml
{
	bool LibLoader::freeLibrary(void * value)
	{
		return ML_FreeLibrary(ML_INSTANCE(value));
	}

	void * LibLoader::loadLibrary(const String & filename)
	{
		return ML_LoadLibrary(filename.c_str());
	}

	void * LibLoader::loadFunction(void * value, const String & name)
	{
		return ML_LoadFunction(ML_INSTANCE(value), name.c_str());
	}

	void * LibLoader::loadPlugin(const String & filename, void * data)
	{
		if (void * lib = loadLibrary(filename))
		{
			if (PluginMainFun fun = (PluginMainFun)loadFunction(lib, "ML_Plugin_Main"))
			{
				fun(data);
			}
			return lib;
		}
		return NULL;
	}
}