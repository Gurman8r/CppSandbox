#include <MemeGraphics/Shader.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>

namespace
{
	GLint CheckMaxTextureUnits()
	{
		GLint maxUnits = 0;

		glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits));

		return maxUnits;
	}

	GLint GetMaxTextureUnits()
	{
		static GLint maxUnits = CheckMaxTextureUnits();

		return maxUnits;
	}
}

namespace ml
{
	// Used to check for errors with uniforms
	struct Shader::UniformBinder
	{
		UniformBinder(Shader& shader, const std::string& name)
			: savedProgram(0)
			, currentProgram(shader.m_program)
			, location(-1)
		{
			if (currentProgram)
			{
				// Enable program object
				glCheck(savedProgram = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

				if (currentProgram != savedProgram)
				{
					glCheck(glUseProgram(currentProgram));
				}

				// Store uniform location for further use outside constructor
				location = shader.getUniformLocation(name);
			}
		}

		~UniformBinder()
		{
			// Disable program object
			if (currentProgram && (currentProgram != savedProgram))
			{
				glCheck(glUseProgram(savedProgram));
			}
		}

		uint32_t	savedProgram;
		uint32_t	currentProgram;
		GLint		location;

		inline operator bool() const
		{
			return location != -1;
		}
	};
}

namespace ml
{
	Shader::Shader()
	{
	}

	Shader::Shader(const Shader & copy)
	{
	}

	Shader::~Shader()
	{
	}


	bool Shader::cleanup()
	{
		return false;
	}

	bool Shader::loadFromFile(const std::string & filename)
	{
		return false;
	}


	void Shader::use() const
	{
		Shader::bind(this);
	}

	int Shader::program() const
	{
		return m_program;
	}

	int Shader::currentTexture() const
	{
		return m_currentTexture;
	}


	void Shader::bind(const Shader * shader, bool tex)
	{
		if (isAvailable())
		{
			if (shader && shader->program())
			{
				glCheck(glUseProgramObjectARB(shader->program()));
			}

			if (tex)
			{
				shader->bindTextures();
			}

			if (shader->currentTexture() != -1)
			{
				glCheck(glUniform1i(shader->currentTexture(), 0));
			}
		}
		else
		{
			glCheck(glUseProgramObjectARB(0));
		}
	}

	bool Shader::isAvailable()
	{
		static bool checked = false;
		static bool available = false;

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

	bool Shader::isGeometryAvailable()
	{
		static bool checked = false;
		static bool available = false;

		if (!checked)
		{
			checked = true;

			available =
				isAvailable() &&
				GL_GEOMETRY_SHADER_ARB;
		}

		return available;
	}
	
	
	void Shader::setUniform(const std::string & name, float value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform1f(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, signed value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform1i(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, unsigned value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform1ui(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, const float * value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec2f & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform2f(getUniformLocation(name), value[0], value[1]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3f & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform3f(getUniformLocation(name), value[0], value[1], value[2]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4f & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform4f(getUniformLocation(name), value[0], value[1], value[2], value[3]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec2i & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform2i(getUniformLocation(name), value[0], value[1]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3i & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform3i(getUniformLocation(name), value[0], value[1], value[2]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4i & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform4i(getUniformLocation(name), value[0], value[1], value[2], value[3]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat3f & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, value.ptr()));
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat4f & value)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.ptr()));
		}
	}
	
	
	void Shader::SetUniformArray(const std::string & name, const float * value, int length)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform1fv(binder.location, length, value));
		}
	}
	
	void Shader::SetUniformArray(const std::string & name, const vec2f * value, int length)
	{
		std::vector<float> contiguous = vec2f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform2fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::SetUniformArray(const std::string & name, const vec3f * value, int length)
	{
		std::vector<float> contiguous = vec3f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform3fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::SetUniformArray(const std::string & name, const vec4f * value, int length)
	{
		std::vector<float> contiguous = vec4f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform4fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::SetUniformArray(const std::string & name, const mat3f * value, int length)
	{
		std::vector<float> contiguous = mat3f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix3fv(binder.location, length, GL_FALSE, &contiguous[0]));
		}
	}
	
	void Shader::SetUniformArray(const std::string & name, const mat4f * value, int length)
	{
		std::vector<float> contiguous = mat4f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix4fv(binder.location, length, GL_FALSE, &contiguous[0]));
		}
	}


	bool Shader::compile(const char * vs, const char * gs, const char * fs)
	{
		if (!isAvailable())
		{
			return false;
		}

		if (gs && !isGeometryAvailable())
		{
			return false;
		}

		if (m_program)
		{
			glCheck(glDeleteObjectARB(m_program));
			m_program = 0;
		}

		m_currentTexture = -1;
		m_textures.clear();
		m_uniforms.clear();

		GLint shaderProgram;
		glCheck(shaderProgram = glCreateProgram());

		// Create the vertex shader if needed
		if (vs)
		{
			// Create and Compile the shader
			GLint vertexShader;
			glCheck(vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER));
			glCheck(glShaderSource(vertexShader, 1, &vs, NULL));
			glCheck(glCompileShader(vertexShader));

			// Check the Compile log
			GLint success;
			glCheck(glGetObjectParameterivARB(vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
			if (success == GL_FALSE)
			{
				char log[1024];
				glCheck(glGetInfoLogARB(vertexShader, sizeof(log), 0, log));
				Debug::LogError("Failed to compile vertex source: {0}", log);
				glCheck(glDeleteObjectARB(vertexShader));
				glCheck(glDeleteObjectARB(shaderProgram));
				return false;
			}

			// Attach the shader to the program, and delete it (not needed anymore)
			glCheck(glAttachObjectARB(shaderProgram, vertexShader));
			glCheck(glDeleteObjectARB(vertexShader));
		}

		// Create the fragment shader if needed
		if (fs)
		{
			// Create and Compile the shader
			GLint fragmentShader;
			glCheck(fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER));
			glCheck(glShaderSource(fragmentShader, 1, &fs, NULL));
			glCheck(glCompileShader(fragmentShader));

			// Check the Compile log
			GLint success;
			glCheck(glGetObjectParameterivARB(fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success));
			if (success == GL_FALSE)
			{
				char log[1024];
				glCheck(glGetInfoLogARB(fragmentShader, sizeof(log), 0, log));
				Debug::LogError("Failed to compile fragment source: {0}", log);
				glCheck(glDeleteObjectARB(fragmentShader));
				glCheck(glDeleteObjectARB(shaderProgram));
				return false;
			}

			// Attach the shader to the program, and delete it (not needed anymore)
			glCheck(glAttachObjectARB(shaderProgram, fragmentShader));
			glCheck(glDeleteObjectARB(fragmentShader));
		}

		// Link the program
		glCheck(glLinkProgram(shaderProgram));

		// Check the link log
		GLint success;
		glCheck(glGetObjectParameterivARB(shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success));
		if (success == GL_FALSE)
		{
			char log[1024];
			glCheck(glGetInfoLogARB(shaderProgram, sizeof(log), 0, log));
			Debug::LogError("Failed to link source: {0}", log);
			glCheck(glDeleteObjectARB(shaderProgram));
			return false;
		}

		m_program = shaderProgram;

		// Force an OpenGL flush, so that the shader will appear updated
		// in all contexts immediately (solves problems in multi-threaded apps)
		glCheck(glFlush());

		return true;
	}
	
	void Shader::bindTextures() const
	{
	}
	
	int Shader::getUniformLocation(const std::string & value)
	{
		return 0;
	}
}