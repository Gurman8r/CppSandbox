#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <MemeCore/Export.h>
#include <MemeCore/String.h>

namespace ml
{
	class ML_CORE_API StringUtility final
	{
	public:
		static string	Replace(const string & src, const string & find, const string & replace);
		static string	Sprintf(const string fmt, ...);	
		static string	ToLower(const string & src);
		static string	ToUpper(const string & src);

		static std::vector<string> Split(const string & src, const string & delims);

	public: // Checks
		static bool		Contains(const string & src, char c);
		static bool		IsAlpha(const string & src);
		static bool		IsAlnum(const string & src);
		static bool		IsBool(const string & src);
		static bool		IsDecimal(const string & src);
		static bool		IsInt(const string & src);
		static bool		IsLower(const string & src);
		static bool		IsName(const string & src);
		static bool		IsNumber(const string & src);
		static bool		IsText(const string & src);
		static bool		IsUpper(const string & src);
		static bool		IsWrap(const string & src, char c);
		static bool		IsWrap(const string & src, char front, char back);
		static bool		IsWrap(const string & src, const string & s);
		static bool		IsWrap(const string & src, const string & front, const string & back);

	public: // Conversions
		static bool		ToBool(const string & src);
		static int32_t	ToInt(const string & src);
		static double	ToDecimal(const string & src);
		static float	ToFloat(const string & src);

		static bool		MakeBool(const string & src, bool& value);
		static bool		MakeInt(const string & src, int32_t& value);
		static bool		MakeDecimal(const string & src, double& value);
		static bool		MakeFloat(const string & src, float& value);

	public: // Templates
		template<typename T, typename ... A>
		inline static string Format(const string & fmt, const T & first, const A & ...args)
		{
			std::stringstream stream;
			stream << first << std::endl;

			int32_t sink[] = { 0, ((void)(stream << args << std::endl), 0)... };
			(void)sink;

			string	temp = fmt;
			for (size_t index = 0; stream.good(); index++)
			{
				const string find = "{" + std::to_string(index) + "}";
				
				string arg;
				if (std::getline(stream, arg))
				{
					for (size_t i = 0; (i = temp.find(find, i)) != string::npos;)
					{
						temp.replace(i, find.size(), arg);
						i += arg.size();
					}
				}
			}
			return temp;
		}

		template <typename T>
		inline static string Format(const T & value)
		{
			return Format(value, "");
		}
	};
}

#endif // !_STRING_UTILITY_H_
