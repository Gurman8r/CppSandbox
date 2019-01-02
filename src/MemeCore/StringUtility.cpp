#include <MemeCore/StringUtility.h>
#include <stdarg.h>
#include <locale>
#include <cctype>

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
	
	std::string	StringUtility::ToLower(const std::string& src)
	{
		std::locale loc;

		std::string toStr = src;

		for (std::string::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::tolower(src[i], loc);
		}

		return toStr;
	}
	std::string	StringUtility::ToUpper(const std::string& src)
	{
		std::locale loc;

		std::string toStr = src;

		for (std::string::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::toupper(src[i], loc);
		}

		return toStr;
	}

	std::vector<std::string> StringUtility::Split(const std::string & src, const std::string & delims)
	{
		std::vector<std::string> list;

		std::string	cpy = src;
		std::size_t	pos = 0;
		std::string	tok;

		while ((pos = cpy.find(delims)) != std::string::npos)
		{
			tok = cpy.substr(0, pos);

			list.push_back(tok);

			cpy.erase(0, pos + delims.length());
		}

		list.push_back(cpy);

		return list;
	}



	bool	StringUtility::Contains(const std::string& src, char c)
	{
		return (src.find(c) != std::string::npos);
	}
	
	bool	StringUtility::IsAlpha(const std::string& src)
	{
		for (char c : src)
		{
			if (!isalpha(c))
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsAlnum(const std::string & src)
	{
		for (char c : src)
		{
			if (!isalpha(c) || !isdigit(c))
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsBool(const std::string& src)
	{
		const std::string low = ToLower(src);
		return
			low == "true" ||
			low == "false" ||
			low == "1" ||
			low == "0";
	}
	
	bool	StringUtility::IsDecimal(const std::string& src)
	{
		if (!src.empty())
		{
			char* endptr = 0;
			strtod(src.c_str(), &endptr);
			if (*endptr != '\0' || endptr == src)
			{
				return false;
			}
			return true;
		}
		return false;
	}
	
	bool	StringUtility::IsInt(const std::string& src)
	{
		if (!src.empty())
		{
			std::string::const_iterator it = src.begin();

			if (*it == '-')
				it++;

			while (it != src.end() && std::isdigit(*it))
				++it;

			return !src.empty() && it == src.end();
		}
		return false;
	}
	
	bool	StringUtility::IsLower(const std::string & src)
	{
		for (auto c : src)
		{
			if (!std::islower(c))
				return false;
		}
		return true;
	}
	
	bool	StringUtility::IsName(const std::string& src)
	{
		if (src.empty())
		{
			return false;
		}
		else if (src.size() == 1)
		{
			return IsAlpha(src);
		}
		else
		{
			if (isalpha(src[0]) || src[0] == '_')
			{
				for (std::size_t i = 1; i < src.size(); i++)
				{
					if (src[i] == '_')
					{
						continue;
					}
					else if (!isalnum(src[i]))
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	
	bool	StringUtility::IsNumber(const std::string& src)
	{
		return IsInt(src) || IsDecimal(src);
	}
	
	bool	StringUtility::IsText(const std::string& src)
	{
		return IsWrap(src, '\"');
	}
	
	bool	StringUtility::IsUpper(const std::string & src)
	{
		for (auto c : src)
		{
			if (!std::isupper(c))
				return false;
		}
		return true;
	}
	
	bool	StringUtility::IsWrap(const std::string& src, char c)
	{
		return IsWrap(src, c, c);
	}
	
	bool	StringUtility::IsWrap(const std::string& src, char front, char back)
	{
		return
			src.length() > 2 &&
			src.front() == front &&
			src.back() == back;
	}
	
	bool	StringUtility::IsWrap(const std::string & src, const std::string & s)
	{
		return IsWrap(src, s, s);
	}
	
	bool	StringUtility::IsWrap(const std::string & src, const std::string & front, const std::string & back)
	{
		if (src.length() >= (front.length() + back.length()))
		{
			return
				(src.substr(0, front.length()) == front) &&
				(src.substr(src.length() - back.length()) == back);
		}

		return false;
	}


	bool	StringUtility::ToBool(const std::string& src)
	{
		return (src == "1" || ToLower(src) == "true");
	}
	
	int		StringUtility::ToInt(const std::string& src)
	{
		return std::stoi(src);
	}
	
	double	StringUtility::ToDecimal(const std::string& src)
	{
		return std::stod(src);
	}
	
	float	StringUtility::ToFloat(const std::string & src)
	{
		return std::stof(src);
	}


	bool	StringUtility::MakeBool(const std::string& src, bool & value)
	{
		if (IsBool(src))
		{
			value = ToBool(src);
			return true;
		}
		value = false;
		return false;
	}
	
	bool	StringUtility::MakeInt(const std::string& src, int & value)
	{
		if (IsInt(src))
		{
			value = std::stoi(src);
			return true;
		}
		value = 0;
		return false;
	}
	
	bool	StringUtility::MakeDecimal(const std::string& src, double & value)
	{
		if (IsDecimal(src))
		{
			value = std::stod(src);
			return true;
		}
		value = 0.0;
		return false;
	}
	
	bool	StringUtility::MakeFloat(const std::string& src, float & value)
	{
		if (IsDecimal(src))
		{
			value = std::stof(src);
			return true;
		}
		value = 0.f;
		return false;
	}
}