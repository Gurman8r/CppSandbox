#include <MemeGraphics/Material.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	Material::Material()
		: m_shader(NULL)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader)
		: m_shader(shader)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader, const UniformSet & uniforms)
		: m_shader(shader)
		, m_uniforms(uniforms)
	{
	}

	Material::Material(const Material & copy)
		: m_shader(copy.m_shader)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Material::~Material()
	{
	}

	bool Material::dispose()
	{
		return true;
	}

	bool Material::loadFromFile(const String & filename)
	{
		static File file;
		if (file.loadFromFile(filename))
		{
			return true;
		}
		return false;
	}


	void Material::serialize(std::ostream & out) const
	{
	}

	void Material::deserialize(std::istream & in)
	{
	}
}