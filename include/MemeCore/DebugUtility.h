#ifndef _DEBUG_UTILITY_H_
#define _DEBUG_UTILITY_H_

#include <MemeCore/StringUtility.h>
#include <iostream>
#include <unordered_map>

#ifdef ML_DEBUG
	#if defined(ML_VisualCpp)
		#define ML_DEBUG_BREAK __debugbreak()
	#else
		#define ML_DEBUG_BREAK raise(SIGTRAP)
	#endif
#endif // ML_DEBUG

namespace ml
{
	inline static std::ostream & out() { return std::cout; }
	inline static std::ostream & err() { return std::cerr; }

	class ML_CORE_API Debug
	{
	public:
		static void terminate(bool areYouSure);

	public:
		static bool Log(const std::string& message);
		static bool LogWarning(const std::string& message);
		static bool LogError(const std::string& message);

	public:
		template<typename T, typename ... A>
		inline static bool Log(const std::string& fmt, const T& arg0, const A&... args)
		{
			return Debug::Log(StringUtility::Format(fmt, arg0, (args)...));
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
