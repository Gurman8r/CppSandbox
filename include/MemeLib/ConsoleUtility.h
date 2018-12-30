#ifndef _CONSOLE_UTILITY_H_
#define _CONSOLE_UTILITY_H_

#include <MemeLib/Export.h>
#include <MemeLib/Enum.h>
#include <iostream>

namespace ml
{
	class ML_API ConsoleUtility final
	{
	public:
		static void setAttribute(const uint16_t & value);
	};

	struct ML_API FG final
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

	struct ML_API BG final
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

	struct ML_API Fmt final
	{
		FG::Color fg;
		BG::Color bg;

		Fmt()
			: fg(FG::Normal)
			, bg(BG::Black)
		{};
		Fmt(FG::Color fg)
			: fg(fg)
			, bg(BG::Black)
		{};
		Fmt(BG::Color bg)
			: fg(FG::Normal)
			, bg(bg)
		{};
		Fmt(FG::Color fg, BG::Color bg)
			: fg(fg)
			, bg(bg)
		{};
	};


	inline Fmt operator|(const BG::Color& bg, const FG::Color& fg)
	{
		return Fmt(fg, bg);
	};

	inline Fmt operator|(const FG::Color& fg, const BG::Color& bg)
	{
		return Fmt(fg, bg);
	};


	inline std::ostream& operator<<(std::ostream& out, const FG::Color& value)
	{
		ConsoleUtility::setAttribute(uint16_t(value));
		return out;
	};

	inline std::ostream& operator<<(std::ostream& out, const BG::Color& value)
	{
		ConsoleUtility::setAttribute(uint16_t(value));
		return out;
	};

	inline std::ostream& operator<<(std::ostream& out, const Fmt& value)
	{
		ConsoleUtility::setAttribute(uint16_t(value.fg) | uint16_t(value.bg));
		return out;
	};
}

#endif // !_CONSOLE_UTILITY_H_
