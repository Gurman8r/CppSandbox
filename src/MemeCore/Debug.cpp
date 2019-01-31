#include <MemeCore/Debug.hpp>
#include <MemeCore/NativeInput.hpp>
#include <MemeCore/IOStream.hpp>

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


	Debug::Status Debug::LogWarning(const String & message)
	{
		ml::cerr
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
		ml::cerr
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
		ml::cout
			<< FMT()
			<< FG::White << "["
			<< FG::Green << " LOG "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return Debug::Success;
	}
}