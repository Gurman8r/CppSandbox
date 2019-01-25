#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <MemeCore/Export.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace ml
{
	class ML_CORE_API StringUtility final
	{
	public:
		static std::string	Replace(const std::string& src, const std::string& find, const std::string& replace);
		static std::string	Sprintf(const std::string fmt, ...);	
		static std::string	ToLower(const std::string& src);
		static std::string	ToUpper(const std::string& src);

		static std::vector<std::string> Split(const std::string& src, const std::string& delims);

	public: // Checks
		static bool		Contains(const std::string& src, char c);
		static bool		IsAlpha(const std::string& src);
		static bool		IsAlnum(const std::string& src);
		static bool		IsBool(const std::string& src);
		static bool		IsDecimal(const std::string& src);
		static bool		IsInt(const std::string& src);
		static bool		IsLower(const std::string& src);
		static bool		IsName(const std::string& src);
		static bool		IsNumber(const std::string& src);
		static bool		IsText(const std::string& src);
		static bool		IsUpper(const std::string& src);
		static bool		IsWrap(const std::string& src, char c);
		static bool		IsWrap(const std::string& src, char front, char back);
		static bool		IsWrap(const std::string& src, const std::string& s);
		static bool		IsWrap(const std::string& src, const std::string& front, const std::string& back);

	public: // Conversions
		static bool		ToBool(const std::string& src);
		static int32_t	ToInt(const std::string& src);
		static double	ToDecimal(const std::string& src);
		static float	ToFloat(const std::string& src);

		static bool		MakeBool(const std::string& src, bool& value);
		static bool		MakeInt(const std::string& src, int32_t& value);
		static bool		MakeDecimal(const std::string& src, double& value);
		static bool		MakeFloat(const std::string& src, float& value);

	public: // Templates
		template<typename T, typename ... A>
		inline static std::string Format(const std::string & fmt, const T & first, const A & ...args)
		{
			std::stringstream stream;
			stream << first << std::endl;

			int32_t sink[] = { 0, ((void)(stream << args << std::endl), 0)... };
			(void)sink;

			std::string	temp = fmt;
			for (size_t index = 0; stream.good(); index++)
			{
				const std::string find = "{" + std::to_string(index) + "}";
				
				std::string arg;
				if (std::getline(stream, arg))
				{
					for (size_t i = 0; (i = temp.find(find, i)) != std::string::npos;)
					{
						temp.replace(i, find.size(), arg);
						i += arg.size();
					}
				}
			}
			return temp;
		}

		template <typename T>
		inline static std::string Format(const T & value)
		{
			return Format(value, "");
		}
	};
}

#endif // !_STRING_UTILITY_H_
