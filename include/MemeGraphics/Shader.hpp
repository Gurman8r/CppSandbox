#ifndef _ML_SHADER_HPP_
#define _ML_SHADER_HPP_

#include <MemeGraphics/UniformSet.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeCore/Transform.hpp>
#include <MemeGraphics/Color.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Shader final
		: public ITrackable
		, public IReadable
		, public IHandle<uint32_t>
	{
	public:
		Shader();
		Shader(const Shader & copy);
		~Shader();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & vs, const String & fs);
		bool loadFromFile(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & source);
		bool loadFromMemory(const String & vs, const String & gs, const String & fs);
		bool loadFromMemory(const String & vs, const String & fs);

	public:
		// Core
		bool bind(bool bindTextures = true) const;
		bool applyUniforms(const UniformSet & value) const;
		bool setUniform(const Uniform & value) const;

	public:
		// Set Uniform
		bool setUniform(const String & name, const float & value) const;
		bool setUniform(const String & name, const int32_t & value) const;
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

	private:
		bool	compile(CString vs, CString gs, CString fs);
		int32_t	getUniformLocation(const String & value) const;
		int32_t	getAttribLocation(const String & value) const;

	private:
		struct	UniformBinder;

		using	TextureTable = Map<int32_t, const Texture *>;
		using	TexturePair  = Pair<int32_t, const Texture *>;
		using	UniformTable = Map<String, int32_t>;

		mutable TextureTable m_textures;
		mutable UniformTable m_uniforms;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SHADER_HPP_