#ifndef _ML_LIB_LOADER_HPP_
#define _ML_LIB_LOADER_HPP_

#include <MemeCore/ITrackable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_Lib ml::LibLoader::getInstance()

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API LibLoader
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

		template <typename T>
		inline T loadFunction(void * value, const String & name)
		{
			return reinterpret_cast<T>(loadFunction(value, name));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIB_LOADER_HPP_