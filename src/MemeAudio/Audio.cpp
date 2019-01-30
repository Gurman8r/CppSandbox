#include <MemeAudio/Audio.h>
#include <MemeCore/Debug.h>
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
			String fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			String errorName;
			String errorDesc;
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
		static bool check = false;
		if (!check && !m_device)
		{
			alCheck(check = (m_device = DEVICE(alcOpenDevice(NULL))));
		}
		return check;
	}

	bool Audio::closeDevice()
	{
		static bool check = false;
		if (!check && m_device)
		{
			alCheck(check = alcCloseDevice(DEVICE(m_device)));
		}
		return check;
	}

	bool Audio::createContext()
	{
		static bool check = false;
		if (!check && m_device && !m_context)
		{
			alCheck(check = (m_context = CONTEXT(alcCreateContext(DEVICE(m_device), NULL))));
		}
		return check;
	}

	bool Audio::makeContextCurrent()
	{
		static bool check = false;
		if (!check && m_context)
		{
			alCheck(check = alcMakeContextCurrent(CONTEXT(m_context)));
		}
		return check;
	}

	bool Audio::init()
	{
		static bool check = false;
		if (!check && !good())
		{
			check =
				openDevice() && 
				createContext() && 
				makeContextCurrent();
		}
		return check;
	}


	bool Audio::genBuffer(AudioBuffer & value, size_t count, size_t size)
	{
		if (good() && count &&  size)
		{
			value.count = count;
			value.size = size;
			for (size_t i = 0; i < size; i++)
			{
				alCheck(alGenBuffers(count, value));
			}
			return true;
		}
		return false;
	}

	bool Audio::genSource(AudioSource & value, size_t count, size_t size)
	{
		if (good() && count &&  size)
		{
			for (size_t i = 0; i < size; i++)
			{
				value.count = count;
				value.size = size;
				alCheck(alGenSources(count, value));
			}
			return true;
		}
		return false;
	}


	void Audio::serialize(std::ostream & out) const
	{
		if (good())
		{

		}
		else
		{
			out << "" << endl;
		}
	}

}