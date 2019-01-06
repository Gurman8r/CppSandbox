#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/InputState.h>

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

	int32_t ConsoleUtility::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		system("pause");
#else
		std::cout << "Press any key to continue..." << std::endl;
		ml::InputState input;
		while (!input.beginStep().endStep().getAnyKey());
#endif
		return exitCode;
	}

	void ConsoleUtility::setAttribute(const uint16_t & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(value));
#else

#endif
	}
}