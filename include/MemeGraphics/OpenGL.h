#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <MemeGraphics/Export.h>

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

// Macro to quickly check every OpenGL API call
#ifdef ML_DEBUG
	// In debug configuration, perform a test on every OpenGL call
	// The do-while loop is needed so that glCheck can be 
	// used as a single statement in if/else branches
	#define glCheck(expr) do { expr; ml::glCheckError(__FILE__, __LINE__, #expr); } while (false)
#else
	// Else, we don't add any overhead
	#define glCheck(expr) (expr)
#endif

namespace ml
{
	void ML_GRAPHICS_API glCheckError(const char* file, unsigned int line, const char* expression);
}

#endif // !_OPENGL_H_