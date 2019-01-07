#include <MemeGraphics/Shader.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/FileSystem.h>

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
					glCheck(glUseProgramObjectARB(currentProgram));
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
				glCheck(glUseProgramObjectARB(savedProgram));
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
		: m_program(0)
		, m_currentTexture(0)
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(const Shader & copy)
		: m_program(copy.m_program)
		, m_currentTexture(copy.m_currentTexture)
		, m_textures(copy.m_textures)
		, m_uniforms(copy.m_uniforms)
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

	bool Shader::loadFromFile(const std::string & vs, const std::string & fs)
	{
		// Read the vertex shader file
		std::vector<char> vertexShader;
		if (!ML_FileSystem.getFileContents(vs, vertexShader))
		{
			Debug::LogError("Failed to open vertex source file \"{0}\"", vs);
			return false;
		}

		// Read the fragment shader file
		std::vector<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			Debug::LogError("Failed to open fragment source file \"{0}\"", fs);
			return false;
		}

		// Compile the shader program
		return compile(&vertexShader[0], NULL, &fragmentShader[0]);
	}

	bool Shader::loadFromFile(const std::string & vs, const std::string & gs, const std::string & fs)
	{
		// Read the vertex shader file
		std::vector<char> vertexShader;
		if (!ML_FileSystem.getFileContents(vs, vertexShader))
		{
			Debug::LogError("Failed to open vertex source file \"{0}\"", vs);
			return false;
		}

		// Read the geometry shader file
		std::vector<char> geometryShader;
		if (!ML_FileSystem.getFileContents(gs, geometryShader))
		{
			Debug::LogError("Failed to open geometry source file \"{0}\"", gs);
			return false;
		}

		// Read the fragment shader file
		std::vector<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			Debug::LogError("Failed to open fragment source file \"{0}\"", fs);
			return false;
		}

		// Compile the shader program
		return compile(&vertexShader[0], &geometryShader[0], &fragmentShader[0]);
	}

	bool Shader::loadFromMemory(const std::string & vs, const std::string & fs)
	{
		return compile(vs.c_str(), NULL, fs.c_str());
	}

	bool Shader::loadFromMemory(const std::string & vs, const std::string & gs, const std::string & fs)
	{
		return compile(vs.c_str(), gs.c_str(), fs.c_str());
	}


	void Shader::use() const
	{
		Shader::bind(this);
	}

	uint32_t Shader::program() const
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
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform1f(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, int32_t value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform1i(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, uint32_t value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform1ui(getUniformLocation(name), value));
		}
	}
	
	void Shader::setUniform(const std::string & name, const float * value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec2f & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform2f(getUniformLocation(name), value[0], value[1]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3f & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform3f(getUniformLocation(name), value[0], value[1], value[2]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4f & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform4f(getUniformLocation(name), value[0], value[1], value[2], value[3]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec2i & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform2i(getUniformLocation(name), value[0], value[1]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3i & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform3i(getUniformLocation(name), value[0], value[1], value[2]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4i & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniform4i(getUniformLocation(name), value[0], value[1], value[2], value[3]));
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat3f & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, value.ptr()));
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat4f & value)
	{
		if (UniformBinder((*this), name))
		{
			glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.ptr()));
		}
	}

	void Shader::setUniform(const std::string & name, const Transform & value)
	{
		setUniform(name, value.matrix());
	}

	void Shader::setUniform(const std::string & name, const Texture * value)
	{
		if (m_program)
		{
			int location = getUniformLocation(name);
			if (location != -1)
			{
				TextureTable::iterator it;
				if ((it = m_textures.find(location)) == m_textures.end())
				{
					GLint maxUnits = GetMaxTextureUnits();

					if ((m_textures.size() + 1) >= static_cast<std::size_t>(maxUnits))
					{
						Debug::LogError("Impossible to use texture \"{0}\" for source: all available texture units are used", name);
						return;
					}

					m_textures[location] = value;
				}
				else
				{
					//it->second = value;
					m_textures[location] = value;
				}
			}
		}
	}
	
	
	void Shader::setUniformArray(const std::string & name, const float * value, int length)
	{
		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform1fv(binder.location, length, value));
		}
	}
	
	void Shader::setUniformArray(const std::string & name, const vec2f * value, int length)
	{
		std::vector<float> contiguous = vec2f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform2fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::setUniformArray(const std::string & name, const vec3f * value, int length)
	{
		std::vector<float> contiguous = vec3f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform3fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::setUniformArray(const std::string & name, const vec4f * value, int length)
	{
		std::vector<float> contiguous = vec4f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniform4fv(binder.location, length, &contiguous[0]));
		}
	}
	
	void Shader::setUniformArray(const std::string & name, const mat3f * value, int length)
	{
		std::vector<float> contiguous = mat3f::Flatten(value, length);

		UniformBinder binder((*this), name);
		if (binder)
		{
			glCheck(glUniformMatrix3fv(binder.location, length, GL_FALSE, &contiguous[0]));
		}
	}
	
	void Shader::setUniformArray(const std::string & name, const mat4f * value, int length)
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
		TextureTable::const_iterator it;
		for (it = m_textures.begin(); it != m_textures.end(); it++)
		{
			it->second->use();
		}
	}
	
	int Shader::getUniformLocation(const std::string & value)
	{
		// Check the cache
		UniformTable::const_iterator it = m_uniforms.find(value);
		if (it != m_uniforms.end())
		{
			// Already in cache, return it
			return it->second;
		}
		else
		{
			// Not in cache, request the location from OpenGL
			int location = glGetUniformLocation(program(), value.c_str());
			m_uniforms.insert(std::make_pair(value, location));

			if (location == -1)
			{
				Debug::LogWarning("Uniform \"{0}\" not found in source", value);
			}

			return location;
		}
	}
}