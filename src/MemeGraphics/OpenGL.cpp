#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#include <GL/glew.h>
#pragma comment (lib, "glew32s.lib")

#define ML_BUFFER_SIZE 1024

namespace ml
{
	// Members
	bool OpenGL::m_good = false;
	bool OpenGL::m_errorPause = false;


	// General

	bool OpenGL::init(bool experimental)
	{
		static bool checked = false;
		if (!checked)
		{
			checked = true;

			glewExperimental = experimental;

			m_good = (glewInit() == GLEW_OK);
		}
		return good();
	}

	bool OpenGL::good()
	{
		return m_good;
	}

	void OpenGL::validate(uint32_t & majorVersion, uint32_t & minorVersion)
	{
		if (good())
		{
			Debug::log("OpenGL version supported by this platform: {0}",
				getString(GL::Version));

			majorVersion = (uint32_t)getInt(GL::MajorVersion);
			minorVersion = (uint32_t)getInt(GL::MinorVersion);
			
			if (getError() == GL::InvalidEnum)
			{
				if (GL::Str version = getString(GL::Version))
				{
					majorVersion = version[0] - '0';
					minorVersion = version[2] - '0';
				}
				else
				{
					Debug::logWarning("Can't get the version number, assuming 1.1");
					majorVersion = 1;
					minorVersion = 1;
				}
			}

			Debug::log("Using OpenGL Version: {0}.{1}",
				majorVersion,
				minorVersion);

			if (!framebuffersAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Mipmap Framebuffers Unavailable");
					warned = true;
				}
			}

			if (!edgeClampAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture Edge Clamp Unavailable");
					warned = true;
				}
			}

			if (!textureSrgbAvailable())
			{
				static bool warned = false;
				if (!warned)
				{
					Debug::logWarning("Texture sRGB Unavailable");
					warned = true;
				}
			}
		}
	}

	
	// Flags

	bool OpenGL::isEnabled(GL::Enum value)
	{
		static bool temp;
		glCheck(temp = glIsEnabled(value));
		return temp;
	}

	bool OpenGL::enable(GL::Enum value, bool check)
	{
		glCheck(glEnable(value));
		return check
			? isEnabled(value)
			: true;
	}

	bool OpenGL::disable(GL::Enum value, bool check)
	{
		glCheck(glDisable(value));
		return check
			? !isEnabled(value)
			: true;
	}


	// Errors

	GL::Err OpenGL::getError()
	{
		return static_cast<GL::Err>(glGetError());
	}

	void OpenGL::errorPause(bool value)
	{
		m_errorPause = value;
	}

	void OpenGL::checkError(GL::Str file, uint32_t line, GL::Str expression)
	{
		// Get the last error
		if (GL::Err errorCode = getError())
		{
			String fileName(file);
			fileName = fileName.substr(fileName.find_last_of("\\/") + 1);

			// Decode the error code
			String errorName;
			String errorDesc;
			switch (errorCode)
			{
			case GL::InvalidEnum:
				errorName = "Invalid GL";
				errorDesc = "An unacceptable value has been specified for an enumerated argument.";
				break;
			case GL::InvalidValue:
				errorName = "Invalid value_type";
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
			
			cerr
				<< FMT()
				<< ml::endl 
				<< FG::Red
				<< "An internal OpenGL call failed in " << fileName << "(" << line << ")"
				<< FG::Yellow	<< ml::endl << "Code: " 
				<< FG::White	<< ml::endl << "\t" << errorCode
				<< FG::Yellow	<< ml::endl << "Expression: "
				<< FG::White	<< ml::endl << "\t" << expression
				<< FG::Yellow	<< ml::endl << "Description:"
				<< FG::White	<< ml::endl << "\t" << errorName
				<< FG::White	<< ml::endl << "\t" << errorDesc
				<< FMT()
				<< ml::endl
				<< ml::endl;

			if (m_errorPause)
			{
				Debug::pause(EXIT_FAILURE);
			}
		}
	}


	// Get

	GL::Str OpenGL::getString(GL::Enum name)
	{
		static const char* temp;
		glCheck(temp = reinterpret_cast<GL::Str>(glGetString(name)));
		return temp;
	}

	GL::Str OpenGL::getString(GL::Enum name, uint32_t index)
	{
		static const char* temp;
		glCheck(temp = reinterpret_cast<GL::Str>(glGetStringi(name, index)));
		return temp;
	}

	bool OpenGL::getBool(GL::Enum name)
	{
		static uint8_t temp;
		glCheck(glGetBooleanv(name, &temp));
		return (bool)temp;
	}

	double OpenGL::getDouble(GL::Enum name)
	{
		static double temp;
		glCheck(glGetDoublev(name, &temp));
		return temp;
	}

	float OpenGL::getFloat(GL::Enum name)
	{
		static float temp;
		glCheck(glGetFloatv(name, &temp));
		return temp;
	}

	int32_t OpenGL::getInt(GL::Enum name)
	{
		static int32_t temp;
		glCheck(glGetIntegerv(name, &temp));
		return temp;
	}


	// Functions

	void OpenGL::activeTexture(GL::TextureID target)
	{
		glCheck(glActiveTexture(target));
	}

	void OpenGL::alphaFunc(GL::Comparison comp, float value)
	{
		glCheck(glAlphaFunc(comp, value));
	}

	void OpenGL::blendFunc(GL::Factor src, GL::Factor dst)
	{
		glCheck(glBlendFunc(src, dst));
	}

	void OpenGL::blendEquation(GL::Equation equation)
	{
		glCheck(glBlendEquation(equation));
	}

	void OpenGL::cullFace(GL::Face face)
	{
		glCheck(glCullFace(face));
	}

	void OpenGL::depthFunc(GL::Comparison cmp)
	{
		glCheck(glDepthFunc(cmp));
	}

	void OpenGL::viewport(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		glCheck(glViewport(x, y, w, h));
	}

	void OpenGL::blendEquationSeparate(GL::Equation lhs, GL::Equation rhs)
	{
		glCheck(glBlendEquationSeparate(lhs, rhs));
	}

	void OpenGL::blendFuncSeparate(GL::Factor sfactorRGB, GL::Factor dfactorRGB, GL::Factor sfactorAlpha, GL::Factor dfactorAlpha)
	{
		glCheck(glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
	}


	// Drawing

	void OpenGL::clear(GL::Mask mask)
	{
		glCheck(glClear(mask));
	}

	void OpenGL::clearColor(float r, float g, float b, float a)
	{
		glCheck(glClearColor(r, g, b, a));
	}

	void OpenGL::drawElements(GL::Mode mode, int32_t count, GL::Type type, const void * indices)
	{
		glCheck(glDrawElements(mode, count, type, indices));
	}

	void OpenGL::drawArrays(GL::Mode mode, int32_t first, int32_t count)
	{
		glCheck(glDrawArrays(mode, first, count));
	}

	void OpenGL::flush()
	{
		glCheck(glFlush());
	}


	void OpenGL::pushMatrix()
	{
		glCheck(glPushMatrix());
	}

	void OpenGL::popMatrix()
	{
		glCheck(glPopMatrix());
	}

	void OpenGL::loadMatrix(const float * value)
	{
		glCheck(glLoadMatrixf(value));
	}

	void OpenGL::begin(GL::Mode mode)
	{
		glCheck(glBegin(mode));
	}

	void OpenGL::end()
	{
		glCheck(glEnd());
	}


	void OpenGL::vertex3f(float x, float y, float z)
	{
		glCheck(glVertex3f(x, y, z));
	}

	void OpenGL::color3f(float r, float g, float b)
	{
		glCheck(glColor3f(r, g, b));
	}

	void OpenGL::color4f(float r, float g, float b, float a)
	{
		glCheck(glColor4f(r, g, b, a));
	}


	// Buffers

	uint32_t OpenGL::genBuffers(uint32_t count)
	{
		static uint32_t temp;
		glCheck(glGenBuffers(1, &temp));
		return temp;
	}

	uint32_t OpenGL::genVertexArrays(uint32_t count)
	{
		static uint32_t temp;
		glCheck(glGenVertexArrays(count, &temp));
		return temp;
	}

	void OpenGL::bindBuffer(GL::Target type, uint32_t buffer)
	{
		glCheck(glBindBuffer(type, buffer));
	}

	void OpenGL::bindVertexArray(uint32_t value)
	{
		glCheck(glBindVertexArray(value));
	}

	void OpenGL::bufferData(GL::Target type, uint32_t size, const void * data, GL::Usage usage)
	{
		glCheck(glBufferData(type, size, data, usage));
	}

	void OpenGL::bufferSubData(GL::Target target, int32_t offset, int32_t size, const void * data)
	{
		glCheck(glBufferSubData(target, offset, size, data));
	}

	void OpenGL::deleteBuffers(uint32_t count, const uint32_t * buffers)
	{
		glCheck(glDeleteBuffers(count, buffers));
	}

	void OpenGL::deleteVertexArrays(uint32_t count, const uint32_t * arrays)
	{
		glCheck(glDeleteVertexArrays(count, arrays));
	}

	void OpenGL::vertexAttribPointer(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width)
	{
#pragma warning(push)
#pragma warning(disable: 4312)
		glCheck(glVertexAttribPointer(
			index,
			size,
			type,
			normalized,
			(stride * width),
			// this causes a warning in x64
			reinterpret_cast<const uint32_t *>(offset * width)));
#pragma warning(pop)
	}

	void OpenGL::enableVertexAttribArray(uint32_t index)
	{
		glCheck(glEnableVertexAttribArray(index));
	}

	int32_t OpenGL::getAttribLocation(uint32_t program, GL::Str name)
	{
		static int32_t temp;
		glCheck(temp = glGetAttribLocation(program, name));
		return temp;
	}


	// Textures

	bool OpenGL::edgeClampAvailable()
	{
		static bool checked = false;
		static bool temp;
		if (!checked)
		{
			checked = true;
			temp = GL_EXT_texture_edge_clamp || GLEW_EXT_texture_edge_clamp;
		}
		return temp;
	}

	bool OpenGL::textureSrgbAvailable()
	{
		static bool checked = false;
		static bool temp;
		if (!checked)
		{
			checked = true;
			temp = GL_EXT_texture_sRGB;
		}
		return temp;
	}

	bool OpenGL::nonPowerOfTwoAvailable()
	{
		static bool checked = false;
		static bool temp;
		if (!checked)
		{
			checked = true;
			temp = GLEW_ARB_texture_non_power_of_two;
		}
		return temp;
	}

	int32_t OpenGL::getMaxTextureUnits()
	{
		static int32_t maxUnits;
		static bool checked = false;
		if (!checked)
		{
			maxUnits = getInt(GL::MaxCombTexImgUnits);
		}
		return maxUnits;
	}

	uint32_t OpenGL::getMaxTextureSize()
	{
		static uint32_t size = 0;
		static bool checked = false;
		if (!checked)
		{
			size = static_cast<uint32_t>(OpenGL::getInt(GL::MaxTextureSize));
		}
		return size;
	}

	uint32_t OpenGL::getValidTextureSize(uint32_t value)
	{
		if (!nonPowerOfTwoAvailable())
		{
			uint32_t powerOfTwo = 1;
			while (powerOfTwo < value)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
		return value;
	}


	uint32_t OpenGL::genTextures(uint32_t count)
	{
		static uint32_t temp;
		glCheck(glGenTextures(count, &temp));
		return temp;
	}

	void OpenGL::deleteTextures(uint32_t count, const uint32_t * id)
	{
		glCheck(glDeleteTextures(count, id));
	}

	void OpenGL::bindTexture(GL::Target target, uint32_t id)
	{
		glCheck(glBindTexture(target, id));
	}

	void OpenGL::texImage2D(GL::Target target, int32_t level, GL::Format internalFormat, int32_t width, int32_t height, int32_t border, GL::Format format, GL::Type type, const void * pixels)
	{
		glCheck(glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels));
	}

	void OpenGL::texSubImage2D(GL::Target target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, GL::Format format, GL::Type type, const void * pixels)
	{
		glCheck(glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels));
	}

	void OpenGL::texParameter(GL::Target target, uint32_t name, GL::TexParam param)
	{
		glCheck(glTexParameteri(target, name, param));
	}

	void OpenGL::getTexImage(GL::Target target, int32_t level, GL::Format format, GL::Type type, void * pixels)
	{
		glCheck(glGetTexImage(target, level, format, type, pixels));
	}

	void OpenGL::generateMipmap(GL::Target target)
	{
		glCheck(glGenerateMipmap(target));
	}

	void OpenGL::pixelStore(uint32_t name, int32_t param)
	{
		glCheck(glPixelStorei(name, 1));
	}

	void OpenGL::scissor(int32_t x, int32_t y, int32_t width, int32_t height)
	{
		glCheck(glScissor(x, y, width, height));
	}


	// Framebuffers

	bool OpenGL::framebuffersAvailable()
	{
		static bool checked = false;
		static bool temp;
		if (!checked)
		{
			checked = true;
			temp = GL_EXT_framebuffer_object && GL_EXT_framebuffer_blit;
		}
		return temp;
	}
	
	uint32_t OpenGL::genFramebuffers(uint32_t count)
	{
		static uint32_t temp;
		glCheck(glGenFramebuffers(count, &temp));
		return temp;
	}

	uint32_t OpenGL::checkFramebufferStatus(GL::Target target)
	{
		static uint32_t temp;
		glCheck(temp = glCheckFramebufferStatus(target));
		return temp;
	}

	void OpenGL::bindFramebuffer(GL::Target target, uint32_t framebuffer)
	{
		glCheck(glBindFramebuffer(target, framebuffer));
	}

	void OpenGL::deleteFramebuffers(uint32_t count, const uint32_t * framebuffers)
	{
		glCheck(glDeleteFramebuffers(count, framebuffers));
	}

	void OpenGL::blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, GL::Mask mask, uint32_t filter)
	{
		glCheck(glBlitFramebuffer(
			srcX0, srcY0, srcX1, srcY1,
			dstX0, dstY0, dstX1, dstY1,
			mask,
			filter));
	}

	void OpenGL::framebufferTexture2D(GL::Target target, GL::Attachment attachment, GL::Target textarget, uint32_t texture, int32_t level)
	{
		glCheck(glFramebufferTexture2D(target, attachment, textarget, texture, level));
	}

	// Renderbuffers

	uint32_t OpenGL::genRenderbuffers(uint32_t count)
	{
		static uint32_t temp;
		glCheck(glGenRenderbuffers(count, &temp));
		return temp;
	}

	void OpenGL::deleteRenderbuffers(uint32_t count, const uint32_t * renderbuffers)
	{
		glCheck(glDeleteRenderbuffers(count, renderbuffers));
	}

	void OpenGL::bindRenderbuffer(GL::Target target, uint32_t renderbuffer)
	{
		glCheck(glBindRenderbuffer(target, renderbuffer));
	}

	void OpenGL::renderbufferStorage(GL::Target target, uint32_t value, int32_t width, int32_t height)
	{
		glCheck(glRenderbufferStorage(target, value, width, height));
	}

	void OpenGL::framebufferRenderbuffer(GL::Target target, uint32_t attachment, GL::Target renderbuffertarget, uint32_t renderbuffer)
	{
		glCheck(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));

	}


	// Shaders

	bool OpenGL::shadersAvailable()
	{
		static bool available = false;
		static bool checked = false;
		if (!checked)
		{
			checked = true;
			available =
				GL_ARB_multitexture &&
				GL_ARB_shading_language_100 &&
				GL_ARB_shader_objects &&
				GL_ARB_vertex_shader &&
				GL_ARB_fragment_shader;
		}
		return available;
	}

	bool OpenGL::geometryShadersAvailable()
	{
		static bool available = false;
		static bool checked = false;
		if (!checked)
		{
			checked = true;
			available = shadersAvailable() && GL_GEOMETRY_SHADER_ARB;
		}
		return available;
	}


	GL::Str OpenGL::getInfoLog(uint32_t obj)
	{
		static char log[ML_BUFFER_SIZE];
		glCheck(glGetInfoLogARB(obj, sizeof(log), 0, log));
		return log;
	}

	uint32_t OpenGL::getProgramHandle(uint32_t name)
	{
		static uint32_t temp;
		glCheck(temp = glGetHandleARB(name));
		return temp;
	}

	uint32_t OpenGL::createProgramObject()
	{
		static uint32_t temp;
		glCheck(temp = glCreateProgramObjectARB());
		return temp;
	}

	uint32_t OpenGL::createShaderObject(GL::ShaderType type)
	{
		static uint32_t temp;
		glCheck(temp = glCreateShaderObjectARB(type));
		return temp;
	}

	int32_t OpenGL::getProgramParameter(int32_t obj, GL::Status param)
	{
		static int32_t temp;
		glCheck(glGetObjectParameterivARB(obj, param, &temp));
		return temp;
	}

	int32_t OpenGL::getProgramiv(uint32_t program, uint32_t name)
	{
		static int32_t temp;
		glCheck(glGetProgramiv(program, name, &temp));
		return temp;
	}

	int32_t OpenGL::getUniformLocation(uint32_t obj, GL::Str name)
	{
		static int32_t temp;
		glCheck(temp = glGetUniformLocation(obj, name));
		return temp;
	}


	void OpenGL::useShader(uint32_t obj)
	{
		glCheck(glUseProgramObjectARB(obj));
	}

	void OpenGL::deleteShader(uint32_t obj)
	{
		glCheck(glDeleteObjectARB(obj));
	}

	void OpenGL::attachShader(uint32_t containerObj, uint32_t obj)
	{
		glCheck(glAttachObjectARB(containerObj, obj));
	}

	void OpenGL::shaderSource(uint32_t obj, int32_t count, GL::Str const * src, const int32_t * length)
	{
		glCheck(glShaderSource(obj, count, src, length));
	}

	void OpenGL::compileShader(uint32_t obj)
	{
		glCheck(glCompileShader(obj));
	}

	void OpenGL::linkShader(uint32_t obj)
	{
		glCheck(glLinkProgram(obj));
	}


	void OpenGL::uniform1f(int32_t location, float value)
	{
		glCheck(glUniform1f(location, value));
	}

	void OpenGL::uniform1i(int32_t location, int32_t value)
	{
		glCheck(glUniform1i(location, value));
	}

	void OpenGL::uniform1u(int32_t location, uint32_t value)
	{
		glCheck(glUniform1ui(location, value));
	}

	void OpenGL::uniform2f(int32_t location, float x, float y)
	{
		glCheck(glUniform2f(location, x, y));
	}

	void OpenGL::uniform3f(int32_t location, float x, float y, float z)
	{
		glCheck(glUniform3f(location, x, y, z));
	}

	void OpenGL::uniform4f(int32_t location, float x, float y, float z, float w)
	{
		glCheck(glUniform4f(location, x, y, z, w));
	}

	void OpenGL::uniform2i(int32_t location, int32_t x, int32_t y)
	{
		glCheck(glUniform2i(location, x, y));
	}

	void OpenGL::uniform3i(int32_t location, int32_t x, int32_t y, int32_t z)
	{
		glCheck(glUniform3i(location, x, y, z));
	}

	void OpenGL::uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w)
	{
		glCheck(glUniform4i(location, x, y, z, w));
	}

	void OpenGL::uniformMatrix2f(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix2fv(location, count, transpose, value));
	}

	void OpenGL::uniformMatrix3f(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix3fv(location, count, transpose, value));
	}

	void OpenGL::uniformMatrix4f(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix4fv(location, count, transpose, value));
	}

	void OpenGL::uniformArray1f(int32_t location, uint32_t count, const float * value)
	{
		glCheck(glUniform1fv(location, count, value));
	}

	void OpenGL::uniformArray2f(int32_t location, uint32_t count, const float * value)
	{
		glCheck(glUniform2fv(location, count, value));
	}

	void OpenGL::uniformArray3f(int32_t location, uint32_t count, const float * value)
	{
		glCheck(glUniform3fv(location, count, value));
	}

	void OpenGL::uniformArray4f(int32_t location, uint32_t count, const float * value)
	{
		glCheck(glUniform4fv(location, count, value));
	}

	void OpenGL::uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix2fv(location, count, GL_FALSE, value));
	}

	void OpenGL::uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix3fv(location, count, GL_FALSE, value));
	}

	void OpenGL::uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, const float * value)
	{
		glCheck(glUniformMatrix4fv(location, count, GL_FALSE, value));
	}

}