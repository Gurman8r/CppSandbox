#include <MemeGraphics/OpenGL.h>
#include <MemeGraphics/GLEW.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/ConsoleUtility.h>
#include <cassert>

namespace ml
{
	bool OpenGL::m_good = false;
	bool OpenGL::m_errorPause = false;

	bool OpenGL::init(bool experimental)
	{
		assert((sizeof(GL::Enum) == sizeof(GLenum)));

		static bool checked = false;
		if (!checked)
		{
			checked = true;

			glewExperimental = experimental;

			m_good = (glewInit() == GLEW_OK);

			Debug::Log("OpenGL version supported by this platform: {0}", 
				getString(GL::Version));
		}
		return m_good;
	}


	GL::Error OpenGL::getError()
	{
		return static_cast<GL::Error>(glGetError());
	}

	void OpenGL::errorPause(bool value)
	{
		m_errorPause = value;
	}

	void OpenGL::checkError(const char * file, unsigned int line, const char * expression)
	{
		// Get the last error
		if (GL::Error errorCode = getError())
		{
			std::string fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			std::string errorName;
			std::string errorDesc;
			switch (errorCode)
			{
			case GL::InvalidEnum:
				errorName = "Invalid GL";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case GL::InvalidValue:
				errorName = "Invalid Value";
				errorDesc = "A numeric argument is out of range.";
				break;
			case GL::InvalidOperation:
				errorName = "Invalid Operation";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case GL::StackOverflow:
				errorName = "Stack Overflow";
				errorDesc = "This command would cause a stack overflow.";
				break;
			case GL::StackUnderflow:
				errorName = "Stack Underflow";
				errorDesc = "This command would cause a stack underflow.";
				break;
			case GL::OutOfMemory:
				errorName = "Out Of Memory";
				errorDesc = "There is not enough memory left to execute the command.";
				break;
			case GL::InvalidFramebufferOperation:
				errorName = "Invalid Framebuffer Operation";
				errorDesc = "The object bound to framebuffer binding is not \"framebuffer complete\".";
				break;
			default:
				errorName = "Unknown error";
				errorDesc = "No description";
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