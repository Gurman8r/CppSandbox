#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	struct Shader::UniformBinder
	{
		uint32_t	cached;
		uint32_t	program;
		int32_t		location;

		UniformBinder(const Shader * shader, const String & name)
			: cached	(NULL)
			, program	(*shader)
			, location	(-1)
		{
			if (program)
			{
				cached = OpenGL::getProgramHandle(GL::ProgramObject);

				if (program != cached)
				{
					OpenGL::useShader(program);
				}

				location = shader->getUniformLocation(name);
			}
		}
		~UniformBinder()
		{
			if (program && (program != cached))
			{
				OpenGL::useShader(cached);
			}
		}

		inline operator bool() const
		{
			return (location != -1);
		}
	};
}

namespace ml
{
	Shader::Shader()
		: IHandle(NULL)
		, m_textures()
		, m_uniforms()
	{
	}

	Shader::Shader(const Shader & copy)
		: IHandle(copy)
		, m_textures(copy.m_textures)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Shader::~Shader()
	{
		cleanup();
	}


	bool Shader::cleanup()
	{
		if ((*this))
		{
			OpenGL::deleteShader((*this));
			
			get_ref() = NULL;

			return true;
		}
		return false;
	}

	bool Shader::loadFromFile(const String & filename)
	{
		String::Stream ss;
		if (ML_FileSystem.getFileContents(filename, ss))
		{
			enum : int8_t { NONE = -1, VERT, FRAG, GEOM, MAX };

			String::Stream source[MAX];
			int8_t type = NONE;

			String line;
			while (std::getline(ss, line))
			{
				if (line.find("#shader") != String::npos)
				{
					if (line.find("vertex") != String::npos)
					{
						type = VERT;
					}
					else if (line.find("fragment") != String::npos)
					{
						type = FRAG;
					}
					else if (line.find("geometry") != String::npos)
					{
						type = GEOM;
					}
				}
				else if(type > NONE)
				{
					source[type] << line << std::endl;
				}
			}

			const String & vs = source[VERT].str();
			const String & gs = source[GEOM].str();
			const String & fs = source[FRAG].str();

			if (!gs.empty())
			{
				return loadFromMemory(vs, gs, fs);
			}
			else
			{
				return loadFromMemory(vs, fs);
			}
		}
		return Debug::logError("Failed to open shader source file \"{0}\"", filename);
	}

	bool Shader::loadFromFile(const String & vs, const String & fs)
	{
		// Read the vertex shader file
		List<char> vertexShader;
		if (!ML_FileSystem.getFileContents(vs, vertexShader))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the fragment shader file
		List<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return compile(&vertexShader[0], NULL, &fragmentShader[0]);
	}

	bool Shader::loadFromFile(const String & vs, const String & gs, const String & fs)
	{
		// Read the vertex shader file
		List<char> vertexShader;
		if (!ML_FileSystem.getFileContents(vs, vertexShader))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the geometry shader file
		List<char> geometryShader;
		if (!ML_FileSystem.getFileContents(gs, geometryShader))
		{
			return Debug::logError("Failed to open geometry source file \"{0}\"", gs);
		}

		// Read the fragment shader file
		List<char> fragmentShader;
		if (!ML_FileSystem.getFileContents(fs, fragmentShader))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return compile(&vertexShader[0], &geometryShader[0], &fragmentShader[0]);
	}

	bool Shader::loadFromMemory(const String & vs, const String & fs)
	{
		return compile(vs.c_str(), NULL, fs.c_str());
	}

	bool Shader::loadFromMemory(const String & vs, const String & gs, const String & fs)
	{
		return compile(vs.c_str(), gs.c_str(), fs.c_str());
	}


	bool Shader::bind(bool bindTextures) const
	{
		return Shader::bind(this, bindTextures);
	}

	bool Shader::bind(const Shader * shader, bool bindTextures)
	{
		if (shader && OpenGL::shadersAvailable())
		{
			if (*shader)
			{
				OpenGL::useShader(*shader);
			}

			if (bindTextures)
			{
				TextureTable::const_iterator it;
				for (it = shader->m_textures.begin(); it != shader->m_textures.end(); it++)
				{
					Texture::bind(it->second);
				}
			}
			return true;
		}
		else
		{
			OpenGL::useShader(NULL);
			return false;
		}
	}
	
	
	bool Shader::setUniform(const String & name, const float & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform1f(u.location, value);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const int32_t & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform1i(u.location, value);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const uint32_t & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform1u(u.location, value);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec2f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform2f(u.location, value[0], value[1]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec3f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform3f(u.location, value[0], value[1], value[2]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec4f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform4f(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec2i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform2i(u.location, value[0], value[1]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec3i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform3i(u.location, value[0], value[1], value[2]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const vec4i & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform4i(u.location, value[0], value[1], value[2], value[3]);
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const mat3f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformMatrix3f(u.location, 1, false, value.ptr());
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const mat4f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformMatrix4f(u.location, 1, false, value.ptr());
		}
		return u;
	}
	bool Shader::setUniform(const String & name, const Texture & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			TextureTable::iterator it;
			if ((it = m_textures.find(u.location)) == m_textures.end())
			{
				static size_t maxUnits = static_cast<size_t>(OpenGL::getMaxTextureUnits());

				if ((m_textures.size() + 1) >= maxUnits)
				{
					Debug::logError("Impossible to use texture \"{0}\" for source: all available texture units are used: {1}", name, maxUnits);
					return u;
				}
			}
			m_textures[u.location] = &value;
		}
		return u;
	}
	
	
	bool Shader::setUniformArray(const String & name, int32_t count, const float * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformArray1f(u.location, count, value);
		}
		return u;
	}
	bool Shader::setUniformArray(const String & name, int32_t count, const vec2f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformArray2f(u.location, count, &vec2f::contiguous(value, count)[0]);
		}
		return u;
	}
	bool Shader::setUniformArray(const String & name, int32_t count, const vec3f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformArray3f(u.location, count, &vec3f::contiguous(value, count)[0]);
		}
		return u;
	}
	bool Shader::setUniformArray(const String & name, int32_t count, const vec4f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformArray4f(u.location, count, &vec4f::contiguous(value, count)[0]);
		}
		return u;
	}
	bool Shader::setUniformArray(const String & name, int32_t count, const mat3f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformMatrix3fv(u.location, count, false, &mat3f::contiguous(value, count)[0]);
		}
		return u;
	}
	bool Shader::setUniformArray(const String & name, int32_t count, const mat4f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformMatrix4fv(u.location, count, false, &mat4f::contiguous(value, count)[0]);
		}
		return u;
	}

	bool Shader::compile(const char * vs, const char * gs, const char * fs)
	{
		if (!OpenGL::shadersAvailable())
		{
			return ml::Debug::logError("Shaders are not available on your system.");
		}

		if (gs && !OpenGL::geometryShadersAvailable())
		{
			return ml::Debug::logError("Geometry shaders are not available on your system.");
		}

		cleanup();

		if (!(*this) && !(get_ref() = OpenGL::createProgramObject()))
		{
			return Debug::logError("Failed creating shader object");
		}

		m_textures.clear();
		m_uniforms.clear();

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
				OpenGL::deleteShader(*this);
				return Debug::logError("Failed to compile vertex source: {0}", log);
			}

			// Attach the shader to the program, and delete it
			OpenGL::attachShader(*this, vertexShader);
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
				OpenGL::deleteShader(*this);
				return Debug::logError("Failed to compile geometry source: {0}", log);
			}

			// Attach the shader to the program, and delete it
			OpenGL::attachShader(*this, geometryShader);
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
				OpenGL::deleteShader(*this);
				return Debug::logError("Failed to compile fragment source: {0}", log);
			}

			// Attach the shader to the program, and delete it
			OpenGL::attachShader(*this, fragmentShader);
			OpenGL::deleteShader(fragmentShader);
		}

		// Link the program
		OpenGL::linkShader(*this);

		// Check the link log
		if (!OpenGL::getProgramParameter(*this, GL::ObjectLinkStatus))
		{
			const char * log = OpenGL::getInfoLog(*this);
			OpenGL::deleteShader(*this);
			return Debug::logError("Failed to link source: {0}", log);
		}

		OpenGL::flush();

		return true;
	}
	
	int32_t Shader::getUniformLocation(const String & value) const
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
			int32_t location = OpenGL::getUniformLocation((*this), value.c_str());
			
			m_uniforms.insert({ value, location });
			
			if (location == -1)
			{
				Debug::logWarning("Uniform \"{0}\" not found in source", value);
			}

			return location;
		}
	}
}