#ifndef _DEBUG_UTILITY_HPP_
#define _DEBUG_UTILITY_HPP_

#include <MemeCore/Console.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		enum : int32_t
		{
			Warning = -1,
			Error	=  0,
			Success = +1,
		};

	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static int32_t	pause(int32_t exitCode);
		static void		terminate();

	public:
		static int32_t logWarning(const String & message); // - 1 true
		static int32_t logError(const String & message);   //   0 false
		static int32_t log(const String & message);		   // + 1 true

	public:
		template <class T, typename ... A>
		inline static int32_t logWarning(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::logWarning(String::Format(fmt, arg0, (args)...));
		}

		template <class T, typename ... A>
		inline static int32_t logError(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::logError(String::Format(fmt, arg0, (args)...));
		}

		template <class T, typename ... A>
		inline static int32_t log(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::log(String::Format(fmt, arg0, (args)...));
		}
	};
	
}

#endif // !_DEBUG_UTILITY_HPP_
