#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	bool Debug::Log(const std::string & message)
	{
		out()
			<< FMT()
			<< FG::White << "["
			<< FG::Green << "  INFO  "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return true;
	}

	bool Debug::LogWarning(const std::string & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Yellow << "  WARN  "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return true;
	}

	bool Debug::LogError(const std::string & message)
	{
		err()
			<< FMT()
			<< FG::White << "["
			<< FG::Red << " ERROR! "
			<< FG::White << "]"
			<< FMT() << " " << message
			<< std::endl;
		return false;
	}
	
}