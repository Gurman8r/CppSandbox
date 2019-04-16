#include <MemeCore/Debug.hpp>
#include <MemeCore/Console.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	void Debug::checkError(CString file, uint32_t line, CString expression)
	{
		if (getInstance().m_errorCode == ML_FAILURE)
		{
			String fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			cerr
				<< FMT()
				<< ml::endl
				<< FG::Red
				<< "A fatal internal call failed in " << fileName << "(" << line << ")"
				<< FG::Yellow << ml::endl << "Code: "
				<< FG::White << ml::endl << "\t" << getInstance().m_errorCode
				<< FG::Yellow << ml::endl << "Expression: "
				<< FG::White << ml::endl << "\t" << expression
				<< FG::Yellow << ml::endl << "Description:"
				<< FG::White << ml::endl << "\t" << getInstance().m_errorName
				<< FG::White << ml::endl << "\t" << getInstance().m_errorDesc
				<< FMT()
				<< ml::endl
				<< ml::endl;

# ifdef ML_DEBUG
			pause(EXIT_FAILURE);
			terminate();
# else
			exit(EXIT_FAILURE);
# endif // ML_DEBUG
		}
		else
		{
			setError(ML_SUCCESS, String(), String());
		}
	}

	void Debug::setError(const int32_t code, const String & name, const String & desc)
	{
		getInstance().m_errorCode = code;
		getInstance().m_errorName = name;
		getInstance().m_errorDesc = desc;
	}

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