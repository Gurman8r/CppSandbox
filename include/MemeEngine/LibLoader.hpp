#ifndef _ML_LIB_LOADER_HPP_
#define _ML_LIB_LOADER_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_Lib ml::LibLoader::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API LibLoader final
		: public ITrackable
		, public ISingleton<LibLoader>
	{
		friend class ISingleton<LibLoader>;

	private:
		LibLoader() {}
		~LibLoader() {}

	public:
		bool	freeLibrary(void * instance);
		void *	loadLibrary(const String & filename);
		void *	loadFunction(void * instance, const String & func);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIB_LOADER_HPP_