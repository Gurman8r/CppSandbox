#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <MemeCore/Export.h>
#include <MemeCore/String.h>

namespace ml
{
	class ML_CORE_API StringUtility final
	{
	public:
		static String	Replace(const String& src, const String& find, const String& replace);
		static String	Sprintf(const String fmt, ...);	
		static String	ToLower(const String& src);
		static String	ToUpper(const String& src);

		static std::vector<String> Split(const String& src, const String& delims);

	public: // Checks
		static bool		Contains(const String& src, char c);
		static bool		IsAlpha(const String& src);
		static bool		IsAlnum(const String& src);
		static bool		IsBool(const String& src);
		static bool		IsDecimal(const String& src);
		static bool		IsInt(const String& src);
		static bool		IsLower(const String& src);
		static bool		IsName(const String& src);
		static bool		IsNumber(const String& src);
		static bool		IsText(const String& src);
		static bool		IsUpper(const String& src);
		static bool		IsWrap(const String& src, char c);
		static bool		IsWrap(const String& src, char front, char back);
		static bool		IsWrap(const String& src, const String& s);
		static bool		IsWrap(const String& src, const String& front, const String& back);

	public: // Conversions
		static bool		ToBool(const String& src);
		static int32_t	ToInt(const String& src);
		static double	ToDecimal(const String& src);
		static float	ToFloat(const String& src);

		static bool		MakeBool(const String& src, bool& value);
		static bool		MakeInt(const String& src, int32_t& value);
		static bool		MakeDecimal(const String& src, double& value);
		static bool		MakeFloat(const String& src, float& value);

	public: // Templates
		template<typename T, typename ... A>
		inline static String Format(const String & fmt, const T & first, const A & ...args)
		{
			std::stringstream stream;
			stream << first << std::endl;

			int32_t sink[] = { 0, ((void)(stream << args << std::endl), 0)... };
			(void)sink;

			String	temp = fmt;
			for (size_t index = 0; stream.good(); index++)
			{
				const String find = "{" + std::to_string(index) + "}";
				
				String arg;
				if (std::getline(stream, arg))
				{
					for (size_t i = 0; (i = temp.find(find, i)) != String::npos;)
					{
						temp.replace(i, find.size(), arg);
						i += arg.size();
					}
				}
			}
			return temp;
		}

		template <typename T>
		inline static String Format(const T & value)
		{
			return Format(value, "");
		}
	};
}

#endif // !_STRING_UTILITY_H_
