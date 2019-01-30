#include <MemeCore/Debug.h>
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

	void Debug::exit(int32_t exitCode)
	{
		std::exit(exitCode);
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		system("pause");
#else
		cout << "Press any key to continue..." << std::Endl;
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


	Debug::Status Debug::LogWarning(const String & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Yellow << " WRN "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Warning;
	}

	Debug::Status Debug::LogError(const String & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Red << " ERR "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Error;
	}
	
	Debug::Status Debug::Log(const String & message)
	{
		out()
			<< FMT()
			<< FG::White << "["
			<< FG::Green << " LOG "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Success;
	}
}