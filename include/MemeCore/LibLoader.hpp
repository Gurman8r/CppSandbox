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

		template <typename _Ret, typename ... _Args>
		inline bool callFunc(void * funHandle, _Ret & result, const _Args & ... args)
		{
			if (auto func = static_cast<_Ret(*)(_Args ...)>(funHandle))
			{
				result = func((args)...);
				return true;
			}
			result = _Ret();
			return false;
		}

		template <typename _Ret, typename ... _Args>
		inline _Ret callFunc(void * value, const _Args & ... args)
		{
			using Func = _Ret(*)(_Args ...);
			Func func;
			assert(func = static_cast<Func>(value));
			return func((args)...);
		}

	private:

	};
}

#endif // !_LIB_LOADER_HPP_
