#include <MemeCore/Console.hpp>
#include <MemeCore/Debug.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <Windows.h>
#	include <conio.h>
# endif // ML_SYSTEM_WINDOWS

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Console::Console()
	{
	}

	Console::~Console()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Console::enableMenuItem(uint32_t item, uint32_t enable)
	{
#ifdef ML_SYSTEM_WINDOWS
		if (HWND console = GetConsoleWindow())
		{
			if (HMENU menu = GetSystemMenu(console, false))
			{
				return EnableMenuItem(menu, item, enable);
			}
		}
		return false;
#else 
		return false;
#endif
	}

	bool Console::setTextAttribute(const uint16_t value)
	{
#ifdef ML_SYSTEM_WINDOWS
		return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
#else 
		return false;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * */
}