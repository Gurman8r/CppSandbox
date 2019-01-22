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
	{
	public:
		Shader();
		Shader(const Shader & copy);
		~Shader();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;
		bool loadFromFile(const std::string & vs, const std::string & fs);
		bool loadFromFile(const std::string & vs, const std::string & gs, const std::string & fs);
		bool loadFromMemory(const std::string& vs, const std::string& fs);
		bool loadFromMemory(const std::string& vs, const std::string& gs, const std::string& fs);

	public:
		Shader &	bind(bool bindTextures = true);
		static void bind(const Shader* shader, bool bindTextures = true);

	public:
		// Set Uniform
		Shader & setUniform(const std::string & name, const float & value);
		Shader & setUniform(const std::string & name, const int32_t & value);
		Shader & setUniform(const std::string & name, const uint32_t & value);
		Shader & setUniform(const std::string & name, const vec2f & value);
		Shader & setUniform(const std::string & name, const vec3f & value);
		Shader & setUniform(const std::string & name, const vec4f & value);
		Shader & setUniform(const std::string & name, const vec2i & value);
		Shader & setUniform(const std::string & name, const vec3i & value);
		Shader & setUniform(const std::string & name, const vec4i & value);
		Shader & setUniform(const std::string & name, const mat3f & value);
		Shader & setUniform(const std::string & name, const mat4f & value);
		Shader & setUniform(const std::string & name, const Texture & value);

		// Set Uniform Array
		Shader & setUniformArray(const std::string & name, int32_t count, const float * value);
		Shader & setUniformArray(const std::string & name, int32_t count, const vec2f * value);
		Shader & setUniformArray(const std::string & name, int32_t count, const vec3f * value);
		Shader & setUniformArray(const std::string & name, int32_t count, const vec4f * value);
		Shader & setUniformArray(const std::string & name, int32_t count, const mat3f * value);
		Shader & setUniformArray(const std::string & name, int32_t count, const mat4f * value);

		Shader & setUniformArray(const std::string & name, const std::vector<float> & value);
		Shader & setUniformArray(const std::string & name, const std::vector<vec2f> & value);
		Shader & setUniformArray(const std::string & name, const std::vector<vec3f> & value);
		Shader & setUniformArray(const std::string & name, const std::vector<vec4f> & value);
		Shader & setUniformArray(const std::string & name, const std::vector<mat3f> & value);
		Shader & setUniformArray(const std::string & name, const std::vector<mat4f> & value);

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
			return setUniformArray(Uniform::Names[id], value);
		};

	public:
		inline const uint32_t & id() const { return m_id; }
		inline operator bool() const { return (bool)id(); }

	private:
		struct	UniformBinder;
		using	TextureTable = std::map<int32_t, const Texture*>;
		using	UniformTable = std::map<std::string, int32_t>;

	private:
		uint32_t m_id;
		TextureTable m_textures;
		mutable UniformTable m_uniforms;
		
		bool		compile(const char* vs, const char* gs, const char* fs);
		int32_t		getUniformLocation(const std::string & value) const;

	};
}

#endif // !_SHADER_H_
