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
		bool loadFromFile(const std::string & vs, const std::string & fs);
		bool loadFromFile(const std::string & vs, const std::string & gs, const std::string & fs);
		bool loadFromMemory(const std::string& vs, const std::string& fs);
		bool loadFromMemory(const std::string& vs, const std::string& gs, const std::string& fs);

		void		use() const;
		uint32_t	program() const;
		int			currentTexture() const;

		static void bind(const Shader* shader, bool tex = true);
		static bool isAvailable();
		static bool isGeometryAvailable();

		// Set Uniform
		void setUniform(const std::string & name, float value);
		void setUniform(const std::string & name, int32_t value);
		void setUniform(const std::string & name, uint32_t value);
		void setUniform(const std::string & name, const float* value);
		void setUniform(const std::string & name, const vec2f & value);
		void setUniform(const std::string & name, const vec3f & value);
		void setUniform(const std::string & name, const vec4f & value);
		void setUniform(const std::string & name, const vec2i & value);
		void setUniform(const std::string & name, const vec3i & value);
		void setUniform(const std::string & name, const vec4i & value);
		void setUniform(const std::string & name, const mat3f & value);
		void setUniform(const std::string & name, const mat4f & value);
		void setUniform(const std::string & name, const Transform & value);
		void setUniform(const std::string & name, const Texture * value);

		// Set Uniform Array
		void setUniformArray(const std::string & name, const float * value, int length);
		void setUniformArray(const std::string & name, const vec2f * value, int length);
		void setUniformArray(const std::string & name, const vec3f * value, int length);
		void setUniformArray(const std::string & name, const vec4f * value, int length);
		void setUniformArray(const std::string & name, const mat3f * value, int length);
		void setUniformArray(const std::string & name, const mat4f * value, int length);

		template <typename T>
		void setUniform(Uniform::ID id, const T & value)
		{
			setUniform(Uniform::UniformNames[id], value);
		};

		template <typename T>
		void setUniformArray(Uniform::ID id, T * value, std::size_t length)
		{
			setUniformArray(Uniform::UniformNames[id], value, length);
		};

	private:
		struct UniformBinder;
		
		using TextureTable = std::map<int, const Texture*>;
		using UniformTable = std::map<std::string, int>;

		bool	compile(const char* vs, const char* gs, const char* fs);
		void	bindTextures() const;
		int		getUniformLocation(const std::string & value);

		uint32_t		m_program;
		int				m_currentTexture;
		TextureTable	m_textures;
		UniformTable	m_uniforms;
	};
}

#endif // !_SHADER_H_
