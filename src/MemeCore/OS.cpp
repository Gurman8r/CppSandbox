#include <MemeCore/OS.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

#ifdef ML_SYSTEM_WINDOWS
#include <windows.h>
#include <shellapi.h>
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	OS::OS()
	{
	}

	OS::~OS() { }

	/* * * * * * * * * * * * * * * * * * * * */

	void OS::onEvent(const IEvent * value)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void * OS::execute(const String & cmd)
	{
		return execute(cmd, String());
	}

	void * OS::execute(const String & cmd, const String & file)
	{
		return execute(cmd, file, String());
	}

	void * OS::execute(const String & cmd, const String & file, const String & args)
	{
		return execute(cmd, file, args, String());
	}

	void * OS::execute(const String & cmd, const String & file, const String & args, const String & path)
	{
#ifdef ML_SYSTEM_WINDOWS
		return ShellExecuteA(
			GetDesktopWindow(), 
			cmd.c_str(), 
			file.c_str(), 
			args.c_str(), 
			path.c_str(), 
			SW_SHOW);
#else
		return NULL;
#endif
	}

	/* * * * * * * * * * * * * * * * * * * * */
}