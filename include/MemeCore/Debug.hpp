#ifndef _DEBUG_UTILITY_HPP_
#define _DEBUG_UTILITY_HPP_

#include <MemeCore/Console.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		enum Status : int32_t
		{
			Warning = -1,
			Error	=  0,
			Success = +1,
		};

	public:
		static int32_t	clear();
		static void		exit(int32_t exitCode);
		static int32_t	pause(int32_t exitCode);
		static void		terminate(bool areYouSure);

	public:
		static Status LogWarning(const String & message);	// - 1 true
		static Status LogError(const String & message);		//   0 false
		static Status Log(const String & message);			// + 1 true

	public:
		template <class T, class ... A>
		inline static Status LogWarning(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogWarning(String().format(fmt, arg0, (args)...));
		}

		template <class T, class ... A>
		inline static Status LogError(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogError(String().format(fmt, arg0, (args)...));
		}

		template <class T, class ... A>
		inline static Status Log(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::Log(String().format(fmt, arg0, (args)...));
		}
	};
	
}

#endif // !_DEBUG_UTILITY_HPP_
