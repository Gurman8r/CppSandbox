#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <MemeGraphics/Export.h>
#include <string>

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
		static bool initGL();

		static void checkError(const char * file, unsigned int line, const char * expression);

		static const char * getVersion();

		static void genVAO(uint32_t count, uint32_t & vao);

		static void setVertexAttribute(uint32_t index, uint32_t size, uint32_t stride);
		static void setVertexAttribute(uint32_t index, uint32_t size, uint32_t stride, void * pointer);
		static void setVertexAttribute(uint32_t index, uint32_t size, uint32_t type, uint32_t normalized, uint32_t stride, void * pointer);

	private:
		static bool m_good;
	};

}

#endif // !_OPENGL_H_