#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/Console.h>
#include <MemeCore/String.h>

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
		template<typename T, typename ... A>
		inline static Status LogWarning(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogWarning(String::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status LogError(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogError(String::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status Log(const String & fmt, const T & arg0, const A &... args)
		{
			return Debug::Log(String::Format(fmt, arg0, (args)...));
		}
	};
	
}

#endif // !_DEBUG_UTILITY_H_
