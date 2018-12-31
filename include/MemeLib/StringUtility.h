#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <MemeLib/Export.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace ml
{
	class ML_API StringUtility final
	{
	public:
		static std::string Replace(const std::string& src, const std::string& find, const std::string& replace);
		static std::string Sprintf(const std::string fmt, ...);

		template<typename T, typename ... A>
		inline static std::string Format(const std::string& fmt, const T& arg0, const A&... args)
		{
			// Move args into stringstream
			std::stringstream ss;
			ss << arg0 << std::endl;

			int sink[] = { 0, ((void)(ss << args << std::endl), 0)... };
			(void)sink;

			std::string	out = fmt;
			for (std::size_t pos = 0; ss.good();)
			{
				std::string find = StringUtility::Sprintf("{%i}", pos);
				std::string replace;
				std::getline(ss, replace);
				out = StringUtility::Replace(out, find, replace);
				pos++;
			}
			return out;
		}

		template <typename T>
		inline static std::string Format(const T & value)
		{
			return Format("{0}", value);
		}
	};
}

#endif // !_STRING_UTILITY_H_
