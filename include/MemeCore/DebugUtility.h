#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/StringUtility.h>
#include <iostream>
#include <unordered_map>

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		inline static std::ostream & out()	{ return std::cout; }
		inline static std::ostream & err()	{ return std::cerr; }
		inline static std::istream & in()	{ return std::cin;	}

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
		static int32_t	pause(int32_t exitCode = EXIT_SUCCESS);
		static void		terminate(bool areYouSure);

	public:
		static Status LogWarning(const std::string & message);	// - 1
		static Status LogError(const std::string & message);	//   0
		static Status Log(const std::string & message);			// + 1

	public:
		template<typename T, typename ... A>
		inline static Status LogWarning(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogWarning(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status LogError(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogError(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static Status Log(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::Log(StringUtility::Format(fmt, arg0, (args)...));
		}
	};
}

#endif // !_DEBUG_UTILITY_H_
