#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	void Debug::terminate(bool areYouSure)
	{
		if (areYouSure)
		{
			std::terminate();
		}
	}

	Debug::ErrorCode Debug::LogWarning(const std::string & message)
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

	Debug::ErrorCode Debug::LogError(const std::string & message)
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
	
	Debug::ErrorCode Debug::Log(const std::string & message)
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