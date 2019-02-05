#ifndef _OPENGL_HPP_
#define _OPENGL_HPP_

#include <MemeGraphics/GL.hpp>

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
	// Wrapper for every OpenGL call used in project
	class ML_GRAPHICS_API OpenGL final
	{
	private: // Members
		static bool m_good;
		static bool m_errorPause;

	public: // General
		static bool		init(bool experimental);
		static bool		good();
		static void		validate(uint32_t & major, uint32_t & minor);

	public: // Flags
		static bool		isEnabled(GL::Enum value);
		static bool		enable(GL::Enum value, bool check = false);
		static bool		disable(GL::Enum value, bool check = false);

	public: // Errors
		static GL::Err	getError();
		static void		errorPause(bool value);
		static void		checkError(GL::Str file, uint32_t line, GL::Str expression);
		
	public: // Get
		static GL::Str	getString(GL::Enum name);
		static GL::Str	getString(GL::Enum name, uint32_t index);
		static bool		getBool(GL::Enum name);
		static double	getDouble(GL::Enum name);
		static float	getFloat(GL::Enum name);
		static int32_t	getInt(GL::Enum name);
		static int32_t*	getIntv(GL::Enum name, int32_t * params);

	public: // Functions
		static void		activeTexture(uint32_t textureID);
		static void		alphaFunc(GL::Comparison comp, float value);
		static void		blendFunc(GL::Factor src, GL::Factor dst);
		static void		blendEquation(GL::Equation equation);
		static void		cullFace(GL::Face value);
		static void		depthFunc(GL::Comparison value);
		static void		viewport(int32_t x, int32_t y, int32_t w, int32_t h);
		static void		blendEquationSeparate(uint32_t lhs, uint32_t rhs);
		static void		blendFuncSeparate(uint32_t sfactorRGB, uint32_t dfactorRGB, uint32_t sfactorAlpha, uint32_t dfactorAlpha);
		static void		polygonMode(GL::Enum face, GL::Enum mode);

	public: // Drawing
		static void		clear(GL::Mask mask);
		static void		clearColor(float r, float g, float b, float a);
		static void		drawElements(GL::Mode mode, int32_t count, GL::Type type, const void * indices);
		static void		drawArrays(GL::Mode mode, int32_t first, int32_t count);
		static void		flush();

		static void		pushMatrix();
		static void		popMatrix();
		static void		loadMatrix(const float * value);
		static void		begin(GL::Mode mode);
		static void		end();

		static void		vertex3f(float x, float y, float z);
		static void		color3f(float r, float g, float b);
		static void		color4f(float r, float g, float b, float a);

	public: // Buffers
		static uint32_t genBuffers(uint32_t count);
		static uint32_t genVertexArrays(uint32_t count);
		static void		bindBuffer(GL::Target type, uint32_t buffer);
		static void		bindVertexArray(uint32_t value);
		static void		bufferData(GL::Target type, uint32_t size, const void * data, GL::Usage usage);
		static void		bufferSubData(GL::Target target, int32_t offset, int32_t size, const void * data);
		static void		deleteBuffers(uint32_t count, const uint32_t * buffers);
		static void		deleteVertexArrays(uint32_t count, const uint32_t * arrays);
		static void		vertexAttribPointer(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, const void * pointer);
		static void		vertexAttribPointer(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width);
		static void		enableVertexAttribArray(uint32_t index);

	public: // Textures
		static int32_t	getMaxTextureUnits();
		static uint32_t	getMaxTextureSize();
		static uint32_t getValidTextureSize(uint32_t value);
		static bool		edgeClampAvailable();
		static bool		textureSrgbAvailable();
		static bool		nonPowerOfTwoAvailable();

		static uint32_t genTextures(uint32_t count);
		static void		deleteTextures(uint32_t count, const uint32_t * id);
		static void		bindTexture(GL::Target target, uint32_t id);
		static void		texImage2D(GL::Target target, int32_t level, GL::Format internalFormat, int32_t width, int32_t height, int32_t border, GL::Format format, GL::Type type, const void * pixels);
		static void		texSubImage2D(GL::Target target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t width, int32_t height, GL::Format format, GL::Type type, const void * pixels);
		static void		texParameter(GL::Target target, uint32_t name, GL::TexParam param);
		static void		getTexImage(GL::Target target, int32_t level, GL::Format format, GL::Type type, void * pixels);
		static void		generateMipmap(GL::Target target);
		static void		pixelStore(uint32_t name, int32_t param);
		static void		scissor(int32_t x, int32_t y, int32_t width, int32_t height);
		static void		bindSampler(uint32_t unit, int32_t sampler);

	public: // Framebuffers
		static bool		framebuffersAvailable();
		static uint32_t genFramebuffers(uint32_t count);
		static uint32_t	checkFramebufferStatus(GL::Target target);
		static void		bindFramebuffer(GL::Target target, uint32_t framebuffer);
		static void		deleteFramebuffers(uint32_t count, const uint32_t * framebuffers);
		static void		blitFramebuffer(int32_t srcX0, int32_t srcY0, int32_t srcX1, int32_t srcY1, int32_t dstX0, int32_t dstY0, int32_t dstX1, int32_t dstY1, GL::Mask mask, uint32_t filter);
		static void		framebufferTexture2D(GL::Target target, GL::Attachment attachment, GL::Target textarget, uint32_t texture, int32_t level);

	public: // Renderbuffers
		static uint32_t genRenderbuffers(uint32_t count);
		static void		deleteRenderbuffers(uint32_t count, const uint32_t * renderbuffers);
		static void		bindRenderbuffer(GL::Target target, uint32_t renderbuffer);
		static void		renderbufferStorage(GL::Target target, uint32_t value, int32_t width, int32_t height);
		static void		framebufferRenderbuffer(GL::Target target, uint32_t attachment, GL::Target renderbuffertarget, uint32_t renderbuffer);

	public: // Shaders
		static bool		shadersAvailable();
		static bool		geometryShadersAvailable();

		static GL::Str	getProgramInfoLog(uint32_t obj);
		static uint32_t getProgramHandle(uint32_t name);
		static uint32_t createProgramObject();
		static uint32_t createShaderObject(GL::ShaderType type);
		static int32_t	getProgramParameter(int32_t obj, GL::Status param);
		static int32_t	getProgramiv(uint32_t program, uint32_t name);
		static int32_t	getAttribLocation(uint32_t program, GL::Str name);
		static int32_t	getUniformLocation(uint32_t program, GL::Str name);

		static void		useShader(uint32_t obj);
		static void		deleteShader(uint32_t obj);
		static void		detachShader(uint32_t containerObj, uint32_t obj);
		static void		attachShader(uint32_t containerObj, uint32_t obj);
		static void		shaderSource(uint32_t obj, int32_t count, GL::Str const * src, const int32_t * length);
		static void		compileShader(uint32_t obj);
		static void		linkShader(uint32_t obj);

		static void		uniform1f(int32_t location, float value);
		static void		uniform1i(int32_t location, int32_t value);
		static void		uniform2f(int32_t location, float x, float y);
		static void		uniform3f(int32_t location, float x, float y, float z);
		static void		uniform4f(int32_t location, float x, float y, float z, float w);
		static void		uniform2i(int32_t location, int32_t x, int32_t y);
		static void		uniform3i(int32_t location, int32_t x, int32_t y, int32_t z);
		static void		uniform4i(int32_t location, int32_t x, int32_t y, int32_t z, int32_t w);
		static void		uniform1fv(int32_t location, uint32_t count, const float * value);
		static void		uniform2fv(int32_t location, uint32_t count, const float * value);
		static void		uniform3fv(int32_t location, uint32_t count, const float * value);
		static void		uniform4fv(int32_t location, uint32_t count, const float * value);
		static void		uniformMatrix2fv(int32_t location, uint32_t count, bool transpose, const float * value);
		static void		uniformMatrix3fv(int32_t location, uint32_t count, bool transpose, const float * value);
		static void		uniformMatrix4fv(int32_t location, uint32_t count, bool transpose, const float * value);
	};

}

#endif // !_OPENGL_HPP_
