#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>
#include <MemeCore/InputState.h>

namespace ml
{
	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return system("cls");
#else
		return system("clear");
#endif
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		system("pause");
#else
		Debug::out() << "Press any key to continue..." << std::endl;
		ml::InputState input;
		while (!input.beginStep().getAnyKey());
#endif
		return exitCode;
	}
	void Debug::terminate(bool areYouSure)
	{
		if (areYouSure)
		{
			std::terminate();
		}
	}

	Debug::Status Debug::LogWarning(const std::string & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Yellow << "  WARN  "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Warning;
	}

	Debug::Status Debug::LogError(const std::string & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Red << " ERROR! "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Error;
	}
	
	Debug::Status Debug::Log(const std::string & message)
	{
		out()
			<< FMT()
			<< FG::White << "["
			<< FG::Green << "  INFO  "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Success;
	}
}