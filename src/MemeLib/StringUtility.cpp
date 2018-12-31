#include <MemeLib/StringUtility.h>
#include <stdarg.h>

#define BUFFER_SIZE 512

namespace ml
{
	std::string StringUtility::Replace(const std::string & src, const std::string & find, const std::string & replace)
	{
		std::string copy = src;
		if (src != "" && find != "")
		{
			for (std::size_t i = 0; (i = copy.find(find, i)) != std::string::npos;)
			{
				copy.replace(i, find.size(), replace);
				i += replace.size();
			}
		}
		return copy;
	}

	std::string StringUtility::Sprintf(const std::string fmt, ...)
	{
		std::vector<char> buf(BUFFER_SIZE);
		va_list args;
		va_start(args, fmt);
		{
			vsnprintf_s(
				&buf[0],
				buf.size(),
				buf.size() + strlen(fmt.c_str()),
				fmt.c_str(),
				args);
		}
		va_end(args);
		return std::string(&buf[0]);
	}
}