#ifndef _CONSOLE_UTILITY_H_
#define _CONSOLE_UTILITY_H_

#include <MemeCore/IntTypes.hpp>
#include <MemeCore/IO.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API Console final
	{
	public:
		static void	setConsoleTextAttribute(const uint16_t & value);
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FG final
	{
		enum Color : uint16_t
		{
			Black		= ML_ENUM_0,
			DarkBlue	= ML_ENUM_1,
			DarkGreen	= ML_ENUM_2,
			DarkCyan	= DarkGreen | DarkBlue,
			DarkRed		= ML_ENUM_4,
			DarkMagenta = DarkRed | DarkBlue,
			DarkYellow	= DarkRed | DarkGreen,
			Normal		= DarkRed | DarkGreen | DarkBlue,
			Gray		= ML_ENUM_8,
			Blue		= Gray | DarkBlue,
			Green		= Gray | DarkGreen,
			Cyan		= Gray | DarkGreen | DarkBlue,
			Red			= Gray | DarkRed,
			Magenta		= Gray | DarkRed | DarkBlue,
			Yellow		= Gray | DarkRed | DarkGreen,
			White		= Gray | DarkRed | DarkGreen | DarkBlue,

			MAX_COLOR = 16
		};

		const static FG::Color Values[FG::MAX_COLOR];
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API BG final
	{
		enum Color : uint16_t
		{
			Black		= ML_ENUM_0,
			DarkBlue	= ML_ENUM_16,
			DarkGreen	= ML_ENUM_32,
			DarkCyan	= DarkGreen | DarkBlue,
			DarkRed		= ML_ENUM_64,
			DarkMagenta = DarkRed | DarkBlue,
			DarkYellow	= DarkRed | DarkGreen,
			Gray		= DarkRed | DarkGreen | DarkBlue,
			DarkGray	= ML_ENUM_128,
			Blue		= DarkGray | DarkBlue,
			Green		= DarkGray | DarkGreen,
			Cyan		= DarkGray | DarkGreen | DarkBlue,
			Red			= DarkGray | DarkRed,
			Magenta		= DarkGray | DarkRed | DarkBlue,
			Yellow		= DarkGray | DarkRed | DarkGreen,
			White		= DarkGray | DarkRed | DarkGreen | DarkBlue,

			MAX_COLOR = 16
		};

		const static BG::Color Values[BG::MAX_COLOR];
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FMT final
	{
		FG::Color fg;
		BG::Color bg;

		FMT()
			: fg(FG::Normal)
			, bg(BG::Black)
		{
		}
		FMT(FG::Color fg)
			: fg(fg)
			, bg(BG::Black)
		{
		}
		FMT(BG::Color bg)
			: fg(FG::Normal)
			, bg(bg)
		{
		}
		FMT(FG::Color fg, BG::Color bg)
			: fg(fg)
			, bg(bg)
		{
		}
		FMT(const FMT & copy)
			: fg(copy.fg)
			, bg(copy.bg)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline FMT operator|(const BG::Color& bg, const FG::Color& fg)
	{
		return FMT(fg, bg);
	};

	inline FMT operator|(const FG::Color& fg, const BG::Color& bg)
	{
		return FMT(fg, bg);
	};

	/* * * * * * * * * * * * * * * * * * * * */

	inline std::ostream& operator<<(std::ostream& out, const FG::Color & value)
	{
		Console::setConsoleTextAttribute(uint16_t(value));
		return out;
	};

	inline std::ostream& operator<<(std::ostream& out, const BG::Color & value)
	{
		Console::setConsoleTextAttribute(uint16_t(value));
		return out;
	};

	inline std::ostream& operator<<(std::ostream& out, const FMT & value)
	{
		Console::setConsoleTextAttribute(uint16_t(value.fg) | uint16_t(value.bg));
		return out;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_CONSOLE_UTILITY_H_
