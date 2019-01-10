#include <MemeGraphics/Shader.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/FileSystem.h>

namespace ml
{
	struct Shader::UniformBinder
	{
		UniformBinder(Shader & shader, const std::string & name)
			: savedProgram(0)
			, currentProgram(shader.m_program)
			, location(-1)
		{
			if (currentProgram)
			{
				// Enable program object
				savedProgram = OpenGL::getProgramHandle(GL::ProgramObject);

				if (currentProgram != savedProgram)
				{
					OpenGL::useShader(currentProgram);
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
				OpenGL::useShader(savedProgram);
			}
		}

		uint32_t	savedProgram;
		uint32_t	currentProgram;
		int32_t		location;

		inline operator bool() const
		{
			return (location != -1);
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
			return Debug::LogError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the fragment shader file
		std::vector<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			return Debug::LogError("Failed to open fragment source file \"{0}\"", fs);
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
			return Debug::LogError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the geometry shader file
		std::vector<char> geometryShader;
		if (!ML_FileSystem.getFileContents(gs, geometryShader))
		{
			return Debug::LogError("Failed to open geometry source file \"{0}\"", gs);
		}

		// Read the fragment shader file
		std::vector<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			return Debug::LogError("Failed to open fragment source file \"{0}\"", fs);
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


	void Shader::use(bool tex) const
	{
		Shader::bind(this, tex);
	}

	void Shader::bind(const Shader * shader, bool tex)
	{
		if (OpenGL::shadersAvailable())
		{
			if (shader && shader->m_program)
			{
				OpenGL::useShader(shader->m_program);
			}

			if (tex)
			{
				shader->bindTextures();
			}

			if (shader->currentTexture() != -1)
			{
				OpenGL::uniform1i(shader->currentTexture(), 0);
			}
		}
		else
		{
			OpenGL::useShader(0);
		}
	}
	
	
	void Shader::setUniform(const std::string & name, float value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1f(u.location, value);
		}
	}
	
	void Shader::setUniform(const std::string & name, int32_t value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1i(u.location, value);
		}
	}
	
	void Shader::setUniform(const std::string & name, uint32_t value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1u(u.location, value);
		}
	}

	void Shader::setUniform(const std::string & name, const vec2f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform2f(u.location, value[0], value[1]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform3f(u.location, value[0], value[1], value[2]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec2i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform2i(u.location, value[0], value[1]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec3i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform3i(u.location, value[0], value[1], value[2]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const vec4i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat3f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrix3f(u.location, 1, false, value.ptr());
		}
	}
	
	void Shader::setUniform(const std::string & name, const mat4f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrix4f(u.location, 1, false, value.ptr());
		}
	}

	void Shader::setUniform(const std::string & name, const Texture * value)
	{
		if (m_program && value)
		{
			int location = getUniformLocation(name);
			if (location != -1)
			{
				TextureTable::iterator it;
				if ((it = m_textures.find(location)) == m_textures.end())
				{
					int32_t maxUnits = OpenGL::getMaxTextureUnits();

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
	
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const float * value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray1f(u.location, count, value);
		}
	}
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const vec2f * value)
	{
		std::vector<float> contiguous = vec2f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray2f(u.location, count, &contiguous[0]);
		}
	}
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const vec3f * value)
	{
		std::vector<float> contiguous = vec3f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray3f(u.location, count, &contiguous[0]);
		}
	}
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const vec4f * value)
	{
		std::vector<float> contiguous = vec4f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray4f(u.location, count, &contiguous[0]);
		}
	}
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const mat3f * value)
	{
		std::vector<float> contiguous = mat3f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrixArray3f(u.location, count, false, &contiguous[0]);
		}
	}
	
	void Shader::setUniformArray(const std::string & name, int32_t count, const mat4f * value)
	{
		std::vector<float> contiguous = mat4f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrixArray4f(u.location, count, false, &contiguous[0]);
		}
	}


	bool Shader::compile(const char * vs, const char * gs, const char * fs)
	{
		if (!OpenGL::shadersAvailable())
		{
			return ml::Debug::LogError("Shaders are not available on your system.");
		}

		if (gs && !OpenGL::geometryShadersAvailable())
		{
			return ml::Debug::LogError("Geometry shaders are not available on your system.");
		}

		if (m_program)
		{
			OpenGL::deleteShader(m_program);
			m_program = 0;
		}

		m_currentTexture = -1;
		m_textures.clear();
		m_uniforms.clear();

		uint32_t shaderProgram = OpenGL::createProgramObject();

		// Create the vertex shader if needed
		if (vs)
		{
			// Create and Compile the shader
			uint32_t vertexShader = OpenGL::createShaderObject(GL::VertexShader);
			OpenGL::shaderSource(vertexShader, 1, &vs, NULL);
			OpenGL::compileShader(vertexShader);

			// Check the Compile log
			if (!OpenGL::getProgramParameter(vertexShader, GL::ObjectCompileStatus))
			{
				const char * log = OpenGL::getInfoLog(vertexShader);
				OpenGL::deleteShader(vertexShader);
				OpenGL::deleteShader(shaderProgram);
				return Debug::LogError("Failed to compile vertex source: {0}", log);
			}

			// Attach the shader to the program, and delete it (not needed anymore)
			OpenGL::attachShader(shaderProgram, vertexShader);
			OpenGL::deleteShader(vertexShader);
		}

		// Create the geometry shader if needed
		if (gs)
		{
			// Create and Compile the shader
			uint32_t geometryShader = OpenGL::createShaderObject(GL::GeometryShader);
			OpenGL::shaderSource(geometryShader, 1, &gs, NULL);
			OpenGL::compileShader(geometryShader);

			// Check the Compile log
			if (!OpenGL::getProgramParameter(geometryShader, GL::ObjectCompileStatus))
			{
				const char * log = OpenGL::getInfoLog(geometryShader);
				OpenGL::deleteShader(geometryShader);
				OpenGL::deleteShader(shaderProgram);
				return Debug::LogError("Failed to compile geometry source: {0}", log);
			}

			// Attach the shader to the program, and delete it (not needed anymore)
			OpenGL::attachShader(shaderProgram, geometryShader);
			OpenGL::deleteShader(geometryShader);
		}

		// Create the fragment shader if needed
		if (fs)
		{
			// Create and Compile the shader
			uint32_t fragmentShader = OpenGL::createShaderObject(GL::FragmentShader);
			OpenGL::shaderSource(fragmentShader, 1, &fs, NULL);
			OpenGL::compileShader(fragmentShader);

			// Check the Compile log
			if (!OpenGL::getProgramParameter(fragmentShader, GL::ObjectCompileStatus))
			{
				const char * log = OpenGL::getInfoLog(fragmentShader);
				OpenGL::deleteShader(fragmentShader);
				OpenGL::deleteShader(shaderProgram);
				return Debug::LogError("Failed to compile fragment source: {0}", log);
			}

			// Attach the shader to the program, and delete it (not needed anymore)
			OpenGL::attachShader(shaderProgram, fragmentShader);
			OpenGL::deleteShader(fragmentShader);
		}

		// Link the program
		OpenGL::linkShader(shaderProgram);

		// Check the link log
		if (!OpenGL::getProgramParameter(shaderProgram, GL::ObjectLinkStatus))
		{
			const char * log = OpenGL::getInfoLog(shaderProgram);
			OpenGL::deleteShader(shaderProgram);
			return Debug::LogError("Failed to link source: {0}", log);
		}

		m_program = shaderProgram;

		OpenGL::flush();

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
			int32_t location = OpenGL::getUniformLocation(m_program, value.c_str());
			
			m_uniforms.insert(std::make_pair(value, location));
			
			if (location == -1)
			{
				Debug::LogWarning("Uniform \"{0}\" not found in source", value);
			}

			return location;
		}
	}
}