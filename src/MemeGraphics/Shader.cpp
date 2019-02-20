#include <MemeGraphics/Shader.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/FileSystem.hpp>

/* * * * * * * * * * * * * * * * * * * * */

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

/* * * * * * * * * * * * * * * * * * * * */

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

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::cleanup()
	{
		m_textures.clear();
		m_uniforms.clear();

		OpenGL::useShader(NULL);
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
		static File file;
		if (file.loadFromFile(filename))
		{
			return loadFromMemory(file);
		}
		return Debug::logError("Failed to open shader source file \"{0}\"", filename);
	}

	bool Shader::loadFromFile(const String & vs, const String & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return compile(vert, NULL, frag);
	}

	bool Shader::loadFromFile(const String & vs, const String & gs, const String & fs)
	{
		// Read the vertex shader file
		static File vert;
		if (!vert.loadFromFile(vs))
		{
			return Debug::logError("Failed to open vertex source file \"{0}\"", vs);
		}

		// Read the geometry shader file
		static File geom;
		if (!geom.loadFromFile(gs))
		{
			return Debug::logError("Failed to open geometry source file \"{0}\"", gs);
		}

		// Read the fragment shader file
		static File frag;
		if (!frag.loadFromFile(fs))
		{
			return Debug::logError("Failed to open fragment source file \"{0}\"", fs);
		}

		// Compile the shader program
		return compile(vert, geom, frag);
	}

	bool Shader::loadFromMemory(const String & source)
	{
		enum { NONE = -1, VERT, FRAG, GEOM, MAX };

		int8_t	srcType = NONE;
		SStream	srcData[MAX];

		SStream	stream(source);
		String	line;
		while (std::getline(stream, line))
		{
			if (line.find("#shader") != String::npos)
			{
				if (line.find("vertex") != String::npos)
				{
					srcType = VERT;
				}
				else if (line.find("fragment") != String::npos)
				{
					srcType = FRAG;
				}
				else if (line.find("geometry") != String::npos)
				{
					srcType = GEOM;
				}
			}
			else if (srcType > NONE)
			{
				srcData[srcType] << line << std::endl;
			}
		}

		return loadFromMemory(
			srcData[VERT].str(),
			srcData[GEOM].str(),
			srcData[FRAG].str());
	}

	bool Shader::loadFromMemory(const String & vs, const String & gs, const String & fs)
	{
		if (gs.empty())
		{
			return loadFromMemory(vs, fs);
		}
		else
		{
			return compile(vs.c_str(), gs.c_str(), fs.c_str());
		}
	}

	bool Shader::loadFromMemory(const String & vs, const String & fs)
	{
		return compile(vs.c_str(), NULL, fs.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::bind(bool bindTextures) const
	{
		if (*this)
		{
			OpenGL::useShader(*this);

			if (bindTextures)
			{
				GL::TextureID texID = GL::Texture0;
				for (const TexturePair & pair : m_textures)
				{
					OpenGL::activeTexture(texID++);

					Texture::bind(pair.second);
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

	bool Shader::applyUniforms(const UniformSet & value) const
	{
		size_t count = 0;
		UniformSet::const_iterator it;
		for (it = value.begin(); it != value.end(); it++)
		{
			if (setUniform(it->second))
			{
				count++;
			}
		}
		return count;
	}

	bool Shader::setUniform(const Uniform & u) const
	{
		if (u.good())
		{
			switch (u.type)
			{
			case Uniform::Int:	return setUniform(u.name, u.get_value<int32_t>());
			case Uniform::Float:return setUniform(u.name, u.get_value<float>());
			case Uniform::Vec2:	return setUniform(u.name, u.get_value<vec2f>());
			case Uniform::Vec3:	return setUniform(u.name, u.get_value<vec3f>());
			case Uniform::Vec4:	return setUniform(u.name, u.get_value<vec4f>());
			case Uniform::Mat3:	return setUniform(u.name, u.get_value<mat3f>());
			case Uniform::Mat4:	return setUniform(u.name, u.get_value<mat4f>());
			case Uniform::Tex:	return setUniform(u.name, u.get_value<Texture>());
			}
		}
		return Debug::logError("Invalid Uniform | {0}", u);
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
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
			OpenGL::uniformMatrix3fv(u.location, 1, false, value.ptr());
		}
		return u;
	}
	
	bool Shader::setUniform(const String & name, const mat4f & value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniformMatrix4fv(u.location, 1, false, value.ptr());
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
					Debug::logError("All available texture units are used: {0}", maxUnits);
					return u;
				}
			}
			m_textures[u.location] = &value;
		}
		return u;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Shader::setUniformArray(const String & name, int32_t count, const float * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform1fv(u.location, count, value);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, int32_t count, const vec2f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform2fv(u.location, count, &vec2f::contiguous(value, count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, int32_t count, const vec3f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform3fv(u.location, count, &vec3f::contiguous(value, count)[0]);
		}
		return u;
	}
	
	bool Shader::setUniformArray(const String & name, int32_t count, const vec4f * value) const
	{
		UniformBinder u(this, name);
		if (u)
		{
			OpenGL::uniform4fv(u.location, count, &vec4f::contiguous(value, count)[0]);
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

	/* * * * * * * * * * * * * * * * * * * * */

	bool Shader::compile(CString vs, CString gs, CString fs)
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

		// Vertex
		uint32_t v = NULL;
		switch (OpenGL::compileShader(v, GL::VertexShader, vs))
		{
		case ML_SUCCESS:
			OpenGL::attachShader((*this), v);
			OpenGL::deleteShader(v);
			break;
		case ML_FAILURE:
			OpenGL::deleteShader((*this));
			return false;
		}

		// Geometry
		uint32_t g = NULL;
		switch (OpenGL::compileShader(g, GL::GeometryShader, gs))
		{
		case ML_SUCCESS:
			OpenGL::attachShader((*this), g);
			OpenGL::deleteShader(g);
			break;
		case ML_FAILURE:
			OpenGL::deleteShader((*this));
			return false;
		}

		// Fragment
		uint32_t f = NULL;
		switch (OpenGL::compileShader(f, GL::FragmentShader, fs))
		{
		case ML_SUCCESS:
			OpenGL::attachShader((*this), f);
			OpenGL::deleteShader(f);
			break;
		case ML_FAILURE:
			OpenGL::deleteShader((*this));
			return false;
		}

		// Link the program
		if (!OpenGL::linkShader(*this))
		{
			CString log = OpenGL::getProgramInfoLog(*this);
			
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
				Debug::logWarning("Unreferenced Uniform: \"{0}\"", value);
			}

			return location;
		}
	}
	
	int32_t Shader::getAttribLocation(const String & value) const
	{
		return OpenGL::getAttribLocation(*this, value.c_str());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}