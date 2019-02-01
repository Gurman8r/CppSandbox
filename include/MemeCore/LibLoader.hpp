#ifndef _LIB_LOADER_HPP_
#define _LIB_LOADER_HPP_

#include <MemeCore/ISingleton.hpp>
#include <MemeCore/ITrackable.hpp>

#define ML_LibLoader ml::LibLoader::getInstance()

namespace ml
{
	class ML_CORE_API LibLoader
		: public ITrackable
		, public ISingleton<LibLoader>
	{
		friend class ISingleton<LibLoader>;

	private:
		LibLoader() {}
		~LibLoader() {}

	public:
		bool	freeLibrary(void * value);
		void *	loadLibrary(const String & filename);
		void *	loadFunction(void * value, const String & name);

		template <typename T, typename ... A>
		inline bool callFunc(void * value, T & result, const A & ... args)
		{
			using Func = T(*)(A ...);
			if (Func func = static_cast<Func>(value))
			{
				result = func((args)...);
				return true;
			}
			result = T();
			return false;
		}

		template <typename T, typename ... A>
		inline T callFunc(void * value, const A & ... args)
		{
			using Func = T(*)(A ...);
			Func func;
			assert(func = static_cast<Func>(value));
			return func((args)...);
		}

	private:

	};
}

#endif // !_LIB_LOADER_HPP_
