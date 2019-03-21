#ifndef _ML_LIB_LOADER_HPP_
#define _ML_LIB_LOADER_HPP_

#include <MemeCore/ITrackable.hpp>

#define ML_Lib ml::LibLoader::getInstance()

#define ML_Plugin_Main ML_Plugin_Main

extern "C"
{
	ML_API_EXPORT int32_t ML_Plugin_Main(void * data);
}

namespace ml
{
	class ML_CORE_API LibLoader
		: public ITrackable
		, public ISingleton<LibLoader>
	{
		friend class ISingleton<LibLoader>;

	public:
		using PluginMainFun = int32_t(*)(void *);

	private:
		LibLoader() {}
		~LibLoader() {}

	public:
		bool	freeLibrary(void * value);
		void *	loadLibrary(const String & filename);
		void *	loadFunction(void * value, const String & name);
		void *	loadPlugin(const String & filename, void * data);

	};
}

#endif // !_ML_LIB_LOADER_HPP_