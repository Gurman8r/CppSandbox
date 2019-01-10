#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/StringUtility.h>
#include <iostream>
#include <unordered_map>

namespace ml
{
	class ML_CORE_API Debug final
	{
	public:
		inline static std::ostream & out() { return std::cout; }
		inline static std::ostream & err() { return std::cerr; }

	public:
		enum ErrorCode : int32_t
		{
			Warning = -1,
			Error	=  0,
			Success = +1,
		};

	public:
		static void terminate(bool areYouSure);

	public:
		static ErrorCode LogWarning(const std::string & message);// - 1
		static ErrorCode LogError(const std::string & message);	 //   0
		static ErrorCode Log(const std::string & message);		 // + 1

	public:
		template<typename T, typename ... A>
		inline static ErrorCode LogWarning(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogWarning(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static ErrorCode LogError(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::LogError(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static ErrorCode Log(const std::string & fmt, const T & arg0, const A &... args)
		{
			return Debug::Log(StringUtility::Format(fmt, arg0, (args)...));
		}
	};
}

#endif // !_DEBUG_UTILITY_H_
