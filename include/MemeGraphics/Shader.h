#ifndef _SHADER_H_
#define _SHADER_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IResource.h>
#include <MemeGraphics/Uniform.h>
#include <MemeGraphics/Texture.h>
#include <MemeGraphics/Transform.h>
#include <MemeGraphics/Color.h>
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
		bool loadFromFile(const string & filename) override;
		bool loadFromFile(const string & vs, const string & fs);
		bool loadFromFile(const string & vs, const string & gs, const string & fs);
		bool loadFromMemory(const string& vs, const string& fs);
		bool loadFromMemory(const string& vs, const string& gs, const string& fs);

	public:
		Shader &	bind(bool bindTextures = true);
		static void bind(const Shader* shader, bool bindTextures = true);

	public:
		// Set Uniform
		Shader & setUniform(const string & name, const float & value);
		Shader & setUniform(const string & name, const int32_t & value);
		Shader & setUniform(const string & name, const uint32_t & value);
		Shader & setUniform(const string & name, const vec2f & value);
		Shader & setUniform(const string & name, const vec3f & value);
		Shader & setUniform(const string & name, const vec4f & value);
		Shader & setUniform(const string & name, const vec2i & value);
		Shader & setUniform(const string & name, const vec3i & value);
		Shader & setUniform(const string & name, const vec4i & value);
		Shader & setUniform(const string & name, const mat3f & value);
		Shader & setUniform(const string & name, const mat4f & value);
		Shader & setUniform(const string & name, const Texture & value);

		// Set Uniform Array
		Shader & setUniformArray(const string & name, int32_t count, const float * value);
		Shader & setUniformArray(const string & name, int32_t count, const vec2f * value);
		Shader & setUniformArray(const string & name, int32_t count, const vec3f * value);
		Shader & setUniformArray(const string & name, int32_t count, const vec4f * value);
		Shader & setUniformArray(const string & name, int32_t count, const mat3f * value);
		Shader & setUniformArray(const string & name, int32_t count, const mat4f * value);

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
		int32_t		getUniformLocation(const string & value) const;
	
	private:
		struct	UniformBinder;
		using	TextureTable = std::map<int32_t, const Texture*>;
		using	UniformTable = std::map<string, int32_t>;

		mutable TextureTable m_textures;
		mutable UniformTable m_uniforms;
	};
}

#endif // !_SHADER_H_
