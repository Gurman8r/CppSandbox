#include <MemeGraphics/OpenGL.h>
#include <MemeCore/StringUtility.h>
#include <MemeCore/ConsoleUtility.h>

namespace ml
{
	void glCheckError(const char * file, unsigned int line, const char * expression)
	{
		// Get the last error
		GLenum errorCode = glGetError();

		if (errorCode != GL_NO_ERROR)
		{
			std::string fileString	= file;
			std::string fileName	= fileString.substr(fileString.find_last_of("\\/") + 1);
			std::string errorName	= "Unknown error";
			std::string errorDesc	= "No description";

			// Decode the error code
			switch (errorCode)
			{
			case GL_INVALID_ENUM:
				errorName = "GL_INVALID_ENUM";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case GL_INVALID_VALUE:
				errorName = "GL_INVALID_VALUE";
				errorDesc = "A numeric argument is out of range.";
				break;
			case GL_INVALID_OPERATION:
				errorName = "GL_INVALID_OPERATION";
				errorDesc = "The specified operation is not allowed in the current state.";
				break;
			case GL_STACK_OVERFLOW:
				errorName = "GL_STACK_OVERFLOW";
				errorDesc = "This command would cause a stack overflow.";
				break;
			case GL_STACK_UNDERFLOW:
				errorName = "GL_STACK_UNDERFLOW";
				errorDesc = "This command would cause a stack underflow.";
				break;
			case GL_OUT_OF_MEMORY:
				errorName = "GL_OUT_OF_MEMORY";
				errorDesc = "There is not enough memory left to execute the command.";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorName = "GL_INVALID_FRAMEBUFFER_OPERATION";
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

#ifdef ML_SYSTEM_WINDOWS
			system("pause");
#else
			// pause ?
#endif

		}
	}
		
}