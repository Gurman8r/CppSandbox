#ifndef _SHADER_H_
#define _SHADER_H_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>
#include <MemeGraphics/Uniform.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Transform.hpp>
#include <MemeGraphics/Color.hpp>
#include <map>

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
		bool loadFromMemory(const String& vs, const String& fs);
		bool loadFromMemory(const String& vs, const String& gs, const String& fs);

	public:
		Shader &	bind(bool bindTextures = true);
		static void bind(const Shader* shader, bool bindTextures = true);

	public:
		// Set Uniform
		Shader & setUniform(const String & name, const float & value);
		Shader & setUniform(const String & name, const int32_t & value);
		Shader & setUniform(const String & name, const uint32_t & value);
		Shader & setUniform(const String & name, const vec2f & value);
		Shader & setUniform(const String & name, const vec3f & value);
		Shader & setUniform(const String & name, const vec4f & value);
		Shader & setUniform(const String & name, const vec2i & value);
		Shader & setUniform(const String & name, const vec3i & value);
		Shader & setUniform(const String & name, const vec4i & value);
		Shader & setUniform(const String & name, const mat3f & value);
		Shader & setUniform(const String & name, const mat4f & value);
		Shader & setUniform(const String & name, const Texture & value);

		// Set Uniform Array
		Shader & setUniformArray(const String & name, int32_t count, const float * value);
		Shader & setUniformArray(const String & name, int32_t count, const vec2f * value);
		Shader & setUniformArray(const String & name, int32_t count, const vec3f * value);
		Shader & setUniformArray(const String & name, int32_t count, const vec4f * value);
		Shader & setUniformArray(const String & name, int32_t count, const mat3f * value);
		Shader & setUniformArray(const String & name, int32_t count, const mat4f * value);

	public:
		template <typename T>
		Shader & setUniform(Uniform::ID id, const T & value)
		{
			return setUniform(Uniform::Names[id], value);
		};

		template <typename T>
		Shader & setUniformArray(Uniform::ID id, int32_t count, T * value)
		{
			return setUniformArray(Uniform::Names[id], value, count);
		};

		template <typename T>
		Shader & setUniformArray(Uniform::ID id, const std::vector<T> & value)
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

#endif // !_SHADER_H_
