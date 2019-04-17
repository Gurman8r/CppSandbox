#include <MemeCore/Debug.hpp>
#include <MemeCore/Console.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#else
		return std::system("clear");
#endif
	}

	void Debug::exit(int32_t exitCode)
	{
		return std::exit(exitCode);
	}

	void Debug::fatal(const String & message)
	{
		logError(message);

#ifdef ML_DEBUG
		terminate();
#else
		exit(EXIT_FAILURE);
#endif // ML_DEBUG
	}

	int32_t Debug::pause(int32_t exitCode)
	{
#ifdef ML_SYSTEM_WINDOWS
		std::system("pause");
#else
		cin.get();
#endif
		return exitCode;
	}

	int32_t	Debug::system(CString cmd)
	{
		return system(cmd, ml::cout);
	}

	int32_t Debug::system(CString cmd, std::ostream & out)
	{
		std::array<char, 128> buffer;
		std::shared_ptr<FILE> file(_popen(cmd, "r"), _pclose);
		if (file)
		{
			while (!feof(file.get()))
			{
				if (fgets(buffer.data(), 128, file.get()) != nullptr)
				{
					out << String(buffer.data());
				}
			}
			return EXIT_SUCCESS;
		}
		return EXIT_FAILURE;
	}

	void Debug::terminate()
	{
		return std::terminate();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const String & Debug::platformTarget()
	{
		static String temp;
#if defined(ML_x64)
		if (!temp) temp = "x64";
#elif defined(ML_x86)
		if (!temp) temp = "x86";
#endif
		return temp;
	}

	const String & Debug::configuration()
	{
		static String temp;
#if defined(ML_DEBUG)
		if (!temp) temp = "Debug";
#else
		if (!temp) temp = "Release";
#endif
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */
}