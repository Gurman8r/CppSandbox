#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/StringUtility.h>
#include <iostream>
#include <unordered_map>

namespace ml
{
	inline static std::ostream & out() { return std::cout; }
	inline static std::ostream & err() { return std::cerr; }

	class ML_CORE_API Debug
	{
	public:
		static bool LogInfo(const std::string& message);
		static bool LogWarning(const std::string& message);
		static bool LogError(const std::string& message);

	public:
		template<typename T, typename ... A>
		inline static bool LogInfo(const std::string& fmt, const T& arg0, const A&... args)
		{
			return Debug::LogInfo(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static bool LogWarning(const std::string& fmt, const T& arg0, const A&... args)
		{
			return Debug::LogWarning(StringUtility::Format(fmt, arg0, (args)...));
		}

		template<typename T, typename ... A>
		inline static bool LogError(const std::string& fmt, const T& arg0, const A&... args)
		{
			return Debug::LogError(StringUtility::Format(fmt, arg0, (args)...));
		}
	};
}

#endif // !_DEBUG_UTILITY_H_
