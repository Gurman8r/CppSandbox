#ifndef _SHADER_H_
#define _SHADER_H_

#include <MemeGraphics/Uniform.h>
#include <MemeGraphics/Texture.h>
#include <MemeCore/ITrackable.h>
#include <MemeCore/IResource.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Vector4.h>
#include <map>

namespace ml
{
	class ML_GRAPHICS_API Shader
		: public ITrackable
		, public IResource
	{
	public:
		enum Type
		{
			Vertex,
			Geometry,
			Fragment,
			MAX_SHADER_TYPE
		};
	public:
		Shader();
		Shader(const Shader & copy);
		~Shader();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		void	use() const;
		int		program() const;
		int		currentTexture() const;

		static void bind(const Shader* shader, bool tex = true);
		static bool isAvailable();
		static bool isGeometryAvailable();

		template <typename T>
		void setUniform(Uniform::ID id, T value)
		{
			setUniform(Uniform::UniformNames[id], value);
		};

		template <typename T>
		void SetUniformArray(Uniform::ID id, T* value, std::size_t length)
		{
			SetUniformArray(Uniform::UniformNames[id], value, length);
		};

		// Set Uniform
		void setUniform(const std::string & name, float value);
		void setUniform(const std::string & name, signed value);
		void setUniform(const std::string & name, unsigned value);
		void setUniform(const std::string & name, const float* value);
		void setUniform(const std::string & name, const vec2f & value);
		void setUniform(const std::string & name, const vec3f & value);
		void setUniform(const std::string & name, const vec4f & value);
		void setUniform(const std::string & name, const vec2i & value);
		void setUniform(const std::string & name, const vec3i & value);
		void setUniform(const std::string & name, const vec4i & value);
		void setUniform(const std::string & name, const mat3f & value);
		void setUniform(const std::string & name, const mat4f & value);

		// Set Uniform Array
		void SetUniformArray(const std::string & name, const float * value, int length);
		void SetUniformArray(const std::string & name, const vec2f * value, int length);
		void SetUniformArray(const std::string & name, const vec3f * value, int length);
		void SetUniformArray(const std::string & name, const vec4f * value, int length);
		void SetUniformArray(const std::string & name, const mat3f * value, int length);
		void SetUniformArray(const std::string & name, const mat4f * value, int length);

	private:
		struct UniformBinder;
		
		using TextureTable = std::map<int, const Texture*>;
		using UniformTable = std::map<std::string, int>;

		bool	compile(const char* vs, const char* gs, const char* fs);
		void	bindTextures() const;
		int		getUniformLocation(const std::string & value);

		int				m_program;
		int				m_currentTexture;
		TextureTable	m_textures;
		UniformTable	m_uniforms;
	};
}

#endif // !_SHADER_H_
