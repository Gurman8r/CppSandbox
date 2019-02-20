#include <MemeCore/Debug.hpp>
#include <MemeCore/Thread.hpp>

namespace ml
{
	int32_t Debug::m_error = ML_SUCCESS;

	bool Debug::checkError(const int32_t value)
	{
		return (m_error == value);
	}

	void Debug::setError(const int32_t value)
	{
		m_error = value;
	}

	int32_t Debug::getError()
	{
		int32_t temp = ML_SUCCESS;
		std::swap(m_error, temp);
		return temp;
	}


	int32_t Debug::clear()
	{
#ifdef ML_SYSTEM_WINDOWS
		return std::system("cls");
#else
		return std::system("clearChildren");
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

	int32_t	Debug::system(const char * cmd)
	{
		ml::Thread([&]() 
		{
			std::array<char, 128> buffer;
			std::shared_ptr<FILE> file(_popen(cmd, "r"), _pclose);
			if (file)
			{
				while (!feof(file.get()))
				{
					if (fgets(buffer.data(), 128, file.get()) != nullptr)
					{
						cout << String(buffer.data());
					}	
				}
			}
		}).launch().join();
		
		return EXIT_SUCCESS;
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