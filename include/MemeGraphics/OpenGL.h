#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <MemeGraphics/GL_Enum.h>

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

// Macro to quickly check every OpenGL API call
#ifdef ML_DEBUG
	// In debug configuration, perform a test on every OpenGL call
	// The do-while loop is needed so that glCheck can be 
	// used as a single statement in if/else branches
	#define glCheck(expr) do { expr; ml::OpenGL::checkError(__FILE__, __LINE__, #expr); } while (false)
#else
	// Else, we don't add any overhead
	#define glCheck(expr) (expr)
#endif

namespace ml
{
	class ML_GRAPHICS_API OpenGL final
	{
	public:
		static bool			initGL(bool experimental);
		
	public:
		static void			errorPause(bool value);
		static Enum::Error	getError();
		static void			checkError(const char * file, unsigned int line, const char * expression);
		
	public:
		static const char * getString(uint32_t name);
		static const char * getString(uint32_t name, uint32_t index);

		static void			getBool(uint32_t name, bool & value);
		static void			getDouble(uint32_t name, double & value);
		static void			getFloat(uint32_t name, float & value);
		static void			getInt(uint32_t name, int32_t & value	);

		static bool			getBool(uint32_t name);
		static double		getDouble(uint32_t name);
		static float		getFloat(uint32_t name);
		static int32_t		getInt(uint32_t name);

	private:
		static bool m_good;
		static bool m_errorPause;
	};

}

#endif // !_OPENGL_H_