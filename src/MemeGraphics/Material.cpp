#include <MemeGraphics/Material.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Material::Material()
		: m_shader	(NULL)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader)
		: m_shader	(shader)
		, m_uniforms()
	{
	}

	Material::Material(const Shader * shader, const UniformSet & uniforms)
		: m_shader	(shader)
		, m_uniforms(uniforms)
	{
	}

	Material::Material(const Material & copy)
		: m_shader	(copy.m_shader)
		, m_uniforms(copy.m_uniforms)
	{
	}

	Material::~Material()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Material::dispose()
	{
		return true;
	}

	bool Material::loadFromFile(const String & filename)
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Material::apply() const
	{
		if (m_shader && (*m_shader))
		{
			for (auto & pair : m_uniforms)
			{
				m_shader->setUniform(pair.second);
			}
			m_shader->bind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}