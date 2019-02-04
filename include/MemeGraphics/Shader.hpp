#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <MemeGraphics/Uniform.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Transform.hpp>
#include <MemeGraphics/Color.hpp>

namespace ml
{
	class ML_GRAPHICS_API Shader final
		: public ITrackable
		, public IResource
		, public IHandle<uint32_t>
	{
	public:
		Shader();
		Shader(const Shader & copy);
		~Shader();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & vs, const String & fs);
		bool loadFromFile(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & vs, const String & fs);
		bool loadFromMemory(const String & vs, const String & gs, const String & fs);

	public:
		bool bind(bool bindTextures = true) const;
		static bool bind(const Shader* shader, bool bindTextures = true);

	public:
		// Set Uniform
		bool setUniform(const String & name, const float & value) const;
		bool setUniform(const String & name, const int32_t & value) const;
		bool setUniform(const String & name, const uint32_t & value) const;
		bool setUniform(const String & name, const vec2f & value) const;
		bool setUniform(const String & name, const vec3f & value) const;
		bool setUniform(const String & name, const vec4f & value) const;
		bool setUniform(const String & name, const vec2i & value) const;
		bool setUniform(const String & name, const vec3i & value) const;
		bool setUniform(const String & name, const vec4i & value) const;
		bool setUniform(const String & name, const mat3f & value) const;
		bool setUniform(const String & name, const mat4f & value) const;
		bool setUniform(const String & name, const Texture & value) const;

		// Set Uniform Array
		bool setUniformArray(const String & name, int32_t count, const float * value) const;
		bool setUniformArray(const String & name, int32_t count, const vec2f * value) const;
		bool setUniformArray(const String & name, int32_t count, const vec3f * value) const;
		bool setUniformArray(const String & name, int32_t count, const vec4f * value) const;
		bool setUniformArray(const String & name, int32_t count, const mat3f * value) const;
		bool setUniformArray(const String & name, int32_t count, const mat4f * value) const;

	public:
		template <class T>
		bool setUniform(Uniform::ID id, const T & value) const
		{
			return setUniform(Uniform::Names[id], value);
		};
		
		template <class T>
		bool setUniformArray(Uniform::ID id, int32_t count, T * value) const
		{
			return setUniformArray(Uniform::Names[id], value, count);
		};
		
		template <class T>
		bool setUniformArray(Uniform::ID id, const std::vector<T> & value) const
		{
			return setUniformArray(Uniform::Names[id], (int32_t)value.size(), &value[0]);
		};

	private:
		bool		compile(const char* vs, const char* gs, const char* fs);
		int32_t		getUniformLocation(const String & value) const;
	
	private:
		struct	UniformBinder;

		using	TextureTable = std::map<int32_t, const Texture*>;
		using	UniformTable = std::map<String, int32_t>;

		mutable TextureTable m_textures;
		mutable UniformTable m_uniforms;
	};
}

#endif // !_SHADER_HPP_
