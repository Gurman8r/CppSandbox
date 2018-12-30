#include <MemeLib/ConsoleUtility.h>

#ifdef ML_SYSTEM_WINDOWS
#include <Windows.h>
#endif // ML_SYSTEM_WINDOWS


namespace ml
{
	void ConsoleUtility::setAttribute(const uint16_t & value)
	{
#ifdef ML_SYSTEM_WINDOWS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(value));
#else

#endif
	}

	void ConsoleUtility::pause()
	{
#ifdef ML_SYSTEM_WINDOWS
		system("pause");
#else
		std::cout << "Press Any Key to Continue..." << std::endl;
#endif
		std::cout << std::endl;
	}
}