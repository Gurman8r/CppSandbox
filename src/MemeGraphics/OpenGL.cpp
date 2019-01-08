#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	bool OpenGL::m_good = false;
	bool OpenGL::m_errorPause = false;

	bool OpenGL::good()
	{
		return m_good;
	}

	bool OpenGL::initGL(bool experimental)
	{
		static bool checked = false;
		if (!checked)
		{
			checked = true;

			glewExperimental = experimental;

			m_good = (glewInit() == GLEW_OK);

			Debug::LogInfo("OpenGL version supported by this platform: {0}", 
				getString(Enum::Version));
		}
		return m_good;
	}


	void OpenGL::errorPause(bool value)
	{
		m_errorPause = value;
	}

	Enum::Error OpenGL::getError()
	{
		return static_cast<Enum::Error>(glGetError());
	}

	void OpenGL::checkError(const char * file, unsigned int line, const char * expression)
	{
		// Get the last error
		Enum::Error errorCode = getError();
		
		if (errorCode != Enum::NoError)
		{
			std::string fileString	= file;
			std::string fileName	= fileString.substr(fileString.find_last_of("\\/") + 1);
			std::string errorName	= "Unknown error";
			std::string errorDesc	= "No description";

			// Decode the error code
			switch (errorCode)
			{
			case Enum::InvalidEnum:
				errorName = "Invalid Enum";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case Enum::InvalidValue:
				errorName = "Invalid Value";
				errorDesc = "A numeric argument is out of range.";
				break;
			case Enum::InvalidOperation:
				errorName = "Invalid Operation";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case Enum::StackOverflow:
				errorName = "Stack Overflow";
				errorDesc = "This command would cause a stack overflow.";
				break;
			case Enum::StackUnderflow:
				errorName = "Stack Underflow";
				errorDesc = "This command would cause a stack underflow.";
				break;
			case Enum::OutOfMemory:
				errorName = "Out Of Memory";
				errorDesc = "There is not enough memory left to execute the command.";
				break;
			case Enum::InvalidFramebufferOperation:
				errorName = "Invalid Framebuffer Operation";
				errorDesc = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
				break;
			}
			
			std::cerr 
				<< FG::Red
				<< "An internal OpenGL call failed in " << fileName << "(" << line << ")"
				<< FG::Yellow	<< std::endl << "Code: " 
				<< FG::White	<< std::endl << "\t" << errorCode
				<< FG::Yellow	<< std::endl << "Expression: "
				<< FG::White	<< std::endl << "\t" << expression
				<< FG::Yellow	<< std::endl << "Description:"
				<< FG::White	<< std::endl << "\t" << errorName
				<< FG::White	<< std::endl << "\t" << errorDesc
				<< std::endl
				<< std::endl << FMT();

			if (m_errorPause)
			{
				ConsoleUtility::pause();
			}
		}
	}


	const char * OpenGL::getString(uint32_t name)
	{
		static const char* temp;
		glCheck(temp = reinterpret_cast<const char *>(glGetString(name)));
		return temp;
	}

	const char * OpenGL::getString(uint32_t name, uint32_t index)
	{
		static const char* temp;
		glCheck(temp = reinterpret_cast<const char *>(glGetStringi(name, index)));
		return temp;
	}


	void OpenGL::getBool(uint32_t name, bool & value)
	{
		static uint8_t temp;
		glCheck(glGetBooleanv(name, &temp));
		value = temp;
	}

	void OpenGL::getDouble(uint32_t name, double & value)
	{
		glCheck(glGetDoublev(name, &value));
	}

	void OpenGL::getFloat(uint32_t name, float & value)
	{
		glCheck(glGetFloatv(name, &value));
	}

	void OpenGL::getInt(uint32_t name, int32_t & value)
	{
		glCheck(glGetIntegerv(name, &value));
	}

	
	bool OpenGL::getBool(uint32_t name)
	{
		static bool temp;
		getBool(name, temp);
		return temp;
	}

	double OpenGL::getDouble(uint32_t name)
	{
		static double temp;
		getDouble(name, temp);
		return temp;
	}

	float OpenGL::getFloat(uint32_t name)
	{
		static float temp;
		getFloat(name, temp);
		return temp;
	}

	int32_t OpenGL::getInt(uint32_t name)
	{
		static int32_t temp;
		getInt(name, temp);
		return temp;
	}
}