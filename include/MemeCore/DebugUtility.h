#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/StringUtility.h>
#include <iostream>
#include <unordered_map>

namespace ml
{
	class ML_CORE_API Debug
	{
	public:
		enum LogLevel : int
		{			
			LV_None		= (0 << 0),
			LV_Messages	= (1 << 1),
			LV_Warnings	= (1 << 2),
			LV_Errors	= (1 << 3),
			LV_All		= (LV_Messages | LV_Warnings | LV_Errors),
		};

		friend Debug::LogLevel operator|(const Debug::LogLevel& lhs, const Debug::LogLevel& rhs)
		{
			return Debug::LogLevel(int(lhs) | int(rhs));
		}

	private:
		static LogLevel s_logLevel;

	public:
		static void Log(const std::string& message);
		static void LogWarning(const std::string& message);
		static bool LogError(const std::string& message);

		template<typename T, typename ... A>
		inline static void Log(const std::string& fmt, const T& arg0, const A&... args)
		{
			Debug::Log(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static void LogWarning(const std::string& fmt, const T& arg0, const A&... args)
		{
			Debug::LogWarning(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static bool LogError(const std::string& fmt, const T& arg0, const A&... args)
		{
			return Debug::LogError(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static void LogUnknown(const std::string& fmt, const T& arg0, const A&... args)
		{
			Debug::LogUnknown(StringUtility::Format(fmt, arg0, (args)...));
		}
	};
}

#endif // !_DEBUG_UTILITY_H_
