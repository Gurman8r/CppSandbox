#include <MemeCore/Debug.hpp>
#include <MemeCore/NativeInput.hpp>

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
		return std::exit(exitCode);
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		system("pause");
#else
		cout << "Press any key to continue..." << std::Endl;
		ml::NativeInput input;
		while (!input.beginStep().getAnyKey());
#endif
		return exitCode;
	}

	void Debug::terminate(bool areYouSure)
	{
		return ((areYouSure)
			? std::terminate()
			: void());
	}


	int32_t Debug::logWarning(const String & message)
	{
		ml::cout
			<< FMT()
			<< FG::White << "["
			<< FG::Yellow << " WRN "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< ml::endl;
		return Debug::Warning;
	}

	int32_t Debug::logError(const String & message)
	{
		ml::cout
			<< FMT()
			<< FG::White << "["
			<< FG::Red << " ERR "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< ml::endl;
		return Debug::Error;
	}
	
	int32_t Debug::log(const String & message)
	{
		ml::cout
			<< FMT()
			<< FG::White << "["
			<< FG::Green << " LOG "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< ml::endl;
		return Debug::Success;
	}
}