#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/DebugUtility.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS


namespace ml
{
	const FG::Color FG::Values[FG::MAX_COLOR] = {
		Black,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		Normal,
		Gray,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White,
	};

	const BG::Color BG::Values[BG::MAX_COLOR] = {
		Black,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		Gray,
		DarkGray,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White,
	};

	void ConsoleUtility::setConsoleTextAttribute(const uint16_t & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(value));
#else

#endif
	}
}