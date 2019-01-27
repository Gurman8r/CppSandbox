#include <MemeAudio/Audio.h>
#include <MemeCore/DebugUtility.h>
#include <AL/al.h>
#include <AL/alc.h>

#define DEVICE(ptr) static_cast<ALCdevice *>(ptr)
#define CONTEXT(ptr) static_cast<ALCcontext *>(ptr)

namespace ml
{
	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
		closeDevice();
	}


	Audio::Error Audio::getError()
	{
		return static_cast<Audio::Error>(alGetError());
	}

	void Audio::checkError(const char * file, uint32_t line, const char * expr)
	{
		if (Error errorCode = getError())
		{
			std::string fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			std::string errorName;
			std::string errorDesc;
			switch (errorCode)
			{
			case Audio::InvalidName:
				errorName = "Invalid Name";
				errorDesc = "An unacceptable value has been specified for a name argument.";
				break;
			case Audio::InvalidEnum:
				errorName = "Invalid Enum";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case Audio::InvalidValue:
				errorName = "Invalid Value";
				errorDesc = "A numeric argument is out of range.";
				break;
			case Audio::InvalidOperation:
				errorName = "Invalid Operation";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case Audio::OutOfMemory:
				errorName = "Out Of Memory";
				errorDesc = "There is not enough memory left to execute the command.";
				break;
			default:
				errorName = "Unknown error";
				errorDesc = "No description";
				break;
			}

			cerr
				<< FMT()
				<< std::endl 
				<< FG::Red
				<< "An internal OpenGL call failed in " << fileName << "(" << line << ")"
				<< FG::Yellow	<< std::endl << "Code: " 
				<< FG::White	<< std::endl << "\t" << errorCode
				<< FG::Yellow	<< std::endl << "Expression: "
				<< FG::White	<< std::endl << "\t" << expr
				<< FG::Yellow	<< std::endl << "Description:"
				<< FG::White	<< std::endl << "\t" << errorName
				<< FG::White	<< std::endl << "\t" << errorDesc
				<< FMT()
				<< std::endl
				<< std::endl;
		}
	}


	bool Audio::openDevice()
	{
		static bool good = false;
		if (!good && !m_device)
		{
			alCheck(good = (m_device = DEVICE(alcOpenDevice(NULL))));
		}
		return good;
	}

	bool Audio::closeDevice()
	{
		static bool good = false;
		if (!good && m_device)
		{
			alCheck(good = alcCloseDevice(DEVICE(m_device)));
		}
		return good;
	}

	bool Audio::createContext()
	{
		static bool good = false;
		if (!good && m_device && !m_context)
		{
			alCheck(good = (m_context = CONTEXT(alcCreateContext(DEVICE(m_device), NULL))));
		}
		return good;
	}

	bool Audio::makeContextCurrent()
	{
		static bool good = false;
		if (!good && m_context)
		{
			alCheck(good = alcMakeContextCurrent(CONTEXT(m_context)));
		}
		return good;
	}

	bool Audio::init()
	{
		static bool good = false;
		if (!good)
		{
			good = openDevice() && createContext() && makeContextCurrent();
		}
		return good;
	}

}