#include <MemeAudio/OpenAL.h>
#include <MemeCore/Debug.h>
#include <AL/al.h>
#include <AL/alc.h>

#define DEVICE(ptr) static_cast<ALCdevice *>(ptr)
#define CONTEXT(ptr) static_cast<ALCcontext *>(ptr)

namespace ml
{
	void * OpenAL::m_device = NULL;
	void * OpenAL::m_context = NULL;


	bool OpenAL::init()
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
	
	bool OpenAL::good()
	{
		return (m_device && m_context);
	}
	
	
	AL::Err OpenAL::getError()
	{
		return static_cast<AL::Err>(alGetError());
	}

	void OpenAL::checkError(const char * file, uint32_t line, const char * expr)
	{
		if (AL::Err errorCode = getError())
		{
			String fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			String errorName;
			String errorDesc;
			switch (errorCode)
			{
			case AL::InvalidName:
				errorName = "Invalid Name";
				errorDesc = "An unacceptable value has been specified for a name argument.";
				break;
			case AL::InvalidEnum:
				errorName = "Invalid Enum";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case AL::InvalidValue:
				errorName = "Invalid Value";
				errorDesc = "A numeric argument is out of range.";
				break;
			case AL::InvalidOperation:
				errorName = "Invalid Operation";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case AL::OutOfMemory:
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
				<< FG::Yellow << std::endl << "Code: "
				<< FG::White << std::endl << "\t" << errorCode
				<< FG::Yellow << std::endl << "Expression: "
				<< FG::White << std::endl << "\t" << expr
				<< FG::Yellow << std::endl << "Description:"
				<< FG::White << std::endl << "\t" << errorName
				<< FG::White << std::endl << "\t" << errorDesc
				<< FMT()
				<< std::endl
				<< std::endl;
		}
	}
	
	
	bool OpenAL::openDevice()
	{
		static bool check = false;
		if (!check && !m_device)
		{
			alCheck(check = (m_device = DEVICE(alcOpenDevice(NULL))));
		}
		return check;
	}
	
	bool OpenAL::closeDevice()
	{
		static bool check = false;
		if (!check && m_device)
		{
			alCheck(check = alcCloseDevice(DEVICE(m_device)));
		}
		return check;
	}
	
	bool OpenAL::createContext()
	{
		static bool check = false;
		if (!check && m_device && !m_context)
		{
			alCheck(check = (m_context = CONTEXT(alcCreateContext(DEVICE(m_device), NULL))));
		}
		return check;
	}
	
	bool OpenAL::makeContextCurrent()
	{
		static bool check = false;
		if (!check && m_context)
		{
			alCheck(check = alcMakeContextCurrent(CONTEXT(m_context)));
		}
		return check;
	}


	void OpenAL::genBuffers(size_t count, size_t size, uint32_t * handle)
	{
		if (good() && count && size)
		{
			for (size_t i = 0; i < size; i++)
			{
				alCheck(alGenBuffers(count, handle));
			}
		}
	}

	void OpenAL::genSources(size_t count, size_t size, uint32_t * handle)
	{
		if (good() && count && size)
		{
			for (size_t i = 0; i < size; i++)
			{
				alCheck(alGenSources(count, handle));
			}
		}
	}
	
}