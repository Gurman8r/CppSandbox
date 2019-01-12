#include <MemeGraphics/Shader.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <MemeCore/FileSystem.h>

namespace ml
{
	struct Shader::UniformBinder
	{
		UniformBinder(Shader & shader, const std::string & name)
			: saved(0)
			, current(shader.m_id)
			, location(-1)
		{
			if (current)
			{
				if (current != (saved = OpenGL::getProgramHandle(GL::ProgramObject)))
				{
					OpenGL::useShader(current);
				}

				location = shader.getUniformLocation(name);
			}
		}
		~UniformBinder()
		{
			if (current && (current != saved))
			{
				OpenGL::useShader(saved);
			}
		}

		uint32_t	saved;
		uint32_t	current;
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
		: m_id(0)
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(const Shader & copy)
		: m_id(copy.m_id)
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
		std::stringstream stream;
		if (ML_FileSystem.getFileContents(filename, stream))
		{
			enum SourceType
			{
				NONE = -1, VERT, FRAG, GEOM, MAX
			};

			std::stringstream	source[SourceType::MAX];
			SourceType			type = SourceType::NONE;

			std::string line;
			while (std::getline(stream, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = SourceType::VERT;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = SourceType::FRAG;
					}
					else if (line.find("geometry") != std::string::npos)
					{
						type = SourceType::GEOM;
					}
				}
				else if(type > SourceType::NONE)
				{
					source[type] << line << '\n';
				}
			}

			const std::string & vs = source[SourceType::VERT].str();
			const std::string & gs = source[SourceType::GEOM].str();
			const std::string & fs = source[SourceType::FRAG].str();

			if (!gs.empty())
			{
				return loadFromMemory(vs, gs, fs);
			}
			else
			{
				return loadFromMemory(vs, fs);
			}
		}
		return Debug::LogError("Failed to open shader source file \"{0}\"", filename);
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


	Shader & Shader::bind(bool bindTextures)
	{
		Shader::bind(this, bindTextures);
		return (*this);
	}

	void Shader::bind(const Shader * shader, bool bindTextures)
	{
		if (shader && OpenGL::shadersAvailable())
		{
			if (shader->m_id)
			{
				OpenGL::useShader(shader->m_id);
			}

			if (bindTextures)
			{
				TextureTable::const_iterator it;
				for (it = shader->m_textures.begin(); it != shader->m_textures.end(); it++)
				{
					Texture::bind(it->second);
				}
			}
		}
		else
		{
			OpenGL::useShader(NULL);
		}
	}
	
	
	Shader & Shader::setUniform(const std::string & name, const float & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1f(u.location, value);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const int32_t & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1i(u.location, value);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const uint32_t & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform1u(u.location, value);
		}
		return (*this);
	}

	Shader & Shader::setUniform(const std::string & name, const vec2f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform2f(u.location, value[0], value[1]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const vec3f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform3f(u.location, value[0], value[1], value[2]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const vec4f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const vec2i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform2i(u.location, value[0], value[1]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const vec3i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform3i(u.location, value[0], value[1], value[2]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const vec4i & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const mat3f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrix3f(u.location, 1, false, value.ptr());
		}
		return (*this);
	}
	
	Shader & Shader::setUniform(const std::string & name, const mat4f & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrix4f(u.location, 1, false, value.ptr());
		}
		return (*this);
	}

	Shader & Shader::setUniform(const std::string & name, const Texture & value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			TextureTable::iterator it;
			if ((it = m_textures.find(u.location)) == m_textures.end())
			{
				static std::size_t maxUnits = static_cast<std::size_t>(OpenGL::getMaxTextureUnits());

				if ((m_textures.size() + 1) >= maxUnits)
				{
					Debug::LogError("Impossible to use texture \"{0}\" for source: all available texture units are used: {1}", name, maxUnits);
					return (*this);
				}
			}
			m_textures[u.location] = &value;
		}
		return (*this);
	}
	
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const float * value)
	{
		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray1f(u.location, count, value);
		}
		return (*this);
	}
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const vec2f * value)
	{
		std::vector<float> contiguous = vec2f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray2f(u.location, count, &contiguous[0]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const vec3f * value)
	{
		std::vector<float> contiguous = vec3f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray3f(u.location, count, &contiguous[0]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const vec4f * value)
	{
		std::vector<float> contiguous = vec4f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformArray4f(u.location, count, &contiguous[0]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const mat3f * value)
	{
		std::vector<float> contiguous = mat3f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrixArray3f(u.location, count, false, &contiguous[0]);
		}
		return (*this);
	}
	
	Shader & Shader::setUniformArray(const std::string & name, int32_t count, const mat4f * value)
	{
		std::vector<float> contiguous = mat4f::Flatten(value, count);

		UniformBinder u((*this), name);
		if (u)
		{
			OpenGL::uniformMatrixArray4f(u.location, count, false, &contiguous[0]);
		}
		return (*this);
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

		if (m_id)
		{
			OpenGL::deleteShader(m_id);
			m_id = 0;
		}

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

			// Attach the shader to the program, and delete it
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

			// Attach the shader to the program, and delete it
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

			// Attach the shader to the program, and delete it
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

		m_id = shaderProgram;

		OpenGL::flush();

		return true;
	}
	
	int32_t Shader::getUniformLocation(const std::string & value)
	{
		// Check the cache
		UniformTable::const_iterator it;

		if ((it = m_uniforms.find(value)) != m_uniforms.end())
		{
			return it->second;
		}
		else
		{
			// Not in cache, request the location from OpenGL
			int32_t location = OpenGL::getUniformLocation(m_id, value.c_str());
			
			m_uniforms.insert(std::make_pair(value, location));
			
			if (location == -1)
			{
				Debug::LogWarning("Uniform \"{0}\" not found in source", value);
			}

			return location;
		}
	}
}