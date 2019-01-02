#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	Debug::LogLevel Debug::s_logLevel = Debug::LV_All;

	void Debug::Log(const std::string & message)
	{
		if (s_logLevel & Debug::LV_Messages)
		{
			std::cout
				<< std::endl
				<< FG::White << "["
				<< FG::Green << "   OK   "
				<< FG::White << "]"
				<< FMT() << message
				<< std::endl;
		}
	}

	void Debug::LogWarning(const std::string & message)
	{
		if (s_logLevel & Debug::LV_Warnings)
		{
			std::cerr
				<< std::endl
				<< FG::White << "["
				<< FG::Yellow << "  WARN  "
				<< FG::White << "]"
				<< FMT() << message
				<< std::endl;
		}
	}

	bool Debug::LogError(const std::string & message)
	{
		if (s_logLevel & Debug::LV_Errors)
		{
			std::cerr
				<< std::endl 
				<< FG::White << "["
				<< FG::Red << " ERROR! "
				<< FG::White << "]"
				<< FMT() << message
				<< std::endl;
		}
		return false;
	}
	
}