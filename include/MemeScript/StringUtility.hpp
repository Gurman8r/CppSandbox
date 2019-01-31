#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <MemeScript/Export.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class ML_SCRIPT_API StringUtility final
	{
	public:
		static String	Replace(const String & src, const String & find, const String & replace);
		static String	Sprintf(const String fmt, ...);	
		static String	ToLower(const String & src);
		static String	ToUpper(const String & src);

		static std::vector<String> Split(const String & src, const String & delims);

	public: // Checks
		static bool		Contains(const String & src, char c);
		static bool		IsAlpha(const String & src);
		static bool		IsAlnum(const String & src);
		static bool		IsBool(const String & src);
		static bool		IsDecimal(const String & src);
		static bool		IsInt(const String & src);
		static bool		IsLower(const String & src);
		static bool		IsName(const String & src);
		static bool		IsNumber(const String & src);
		static bool		IsText(const String & src);
		static bool		IsUpper(const String & src);
		static bool		IsWrap(const String & src, char c);
		static bool		IsWrap(const String & src, char front, char back);
		static bool		IsWrap(const String & src, const String & s);
		static bool		IsWrap(const String & src, const String & front, const String & back);

	public: // Conversions
		static bool		ToBool(const String & src);
		static int32_t	ToInt(const String & src);
		static double	ToDecimal(const String & src);
		static float	ToFloat(const String & src);

		static bool		MakeBool(const String & src, bool& value);
		static bool		MakeInt(const String & src, int32_t& value);
		static bool		MakeDecimal(const String & src, double& value);
		static bool		MakeFloat(const String & src, float& value);
	};
}

#endif // !_STRING_UTILITY_H_
