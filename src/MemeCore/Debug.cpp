#include <MemeCore/Debug.hpp>

namespace ml
{
	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return system("cls");
#else
		return system("clearChildren");
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
		cin.get();
#endif
		return exitCode;
	}

	void Debug::terminate()
	{
		return std::terminate();
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
		return ML_WARNING;
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
		return ML_FAILURE;
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
		return ML_SUCCESS;
	}
}