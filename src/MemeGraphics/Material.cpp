#include <MemeGraphics/Material.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeCore/File.hpp>
#include <MemeGraphics/Uni.hpp>

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

	Material::Material(const Shader * shader, const UniformList & uniforms)
		: m_shader	(shader)
		, m_uniforms()
	{
		for (auto & e : uniforms)
		{
			m_uniforms[e->name] = e;
		}
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
		for (auto & pair : m_uniforms)
		{
			ITrackable::operator delete(pair.second);
		}
		m_uniforms.clear();
		return m_uniforms.empty();
	}

	bool Material::loadFromFile(const String & filename)
	{
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Material::bind() const
	{
		if (m_shader && (*m_shader))
		{
			for (const auto & pair : m_uniforms)
			{
				if (!pair.second || !pair.second->name || !pair.second->type)
					continue;

				switch (pair.second->type)
				{
				case uni_base::Float:
					if (auto u = dynamic_cast<const uni_cp_flt *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_flt *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_flt *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Int:
					if (auto u = dynamic_cast<const uni_cp_int *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_int *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_int *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Vec2:
					if (auto u = dynamic_cast<const uni_cp_vec2 *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_vec2 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec2 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Vec3:
					if (auto u = dynamic_cast<const uni_cp_vec3 *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_vec3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Vec4:
					if (auto u = dynamic_cast<const uni_cp_vec4 *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_vec4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_vec4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Col4:
					if (auto u = dynamic_cast<const uni_cp_col *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_col *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_col *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Mat3:
					if (auto u = dynamic_cast<const uni_cp_mat3 *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_mat3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat3 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Mat4:
					if (auto u = dynamic_cast<const uni_cp_mat4 *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					else if (auto u = dynamic_cast<const uni_cr_mat4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					else if (auto u = dynamic_cast<const uni_mat4 *>(pair.second))
						m_shader->setUniform(u->name, u->data);
					break;

				case uni_base::Tex2D:
					if (auto u = dynamic_cast<const uni_cp_tex *>(pair.second))
						m_shader->setUniform(u->name, *u->data);
					break;
				}
			}

			m_shader->bind();
			return true;
		}
		return false;
	}

	void Material::unbind() const
	{
		if (m_shader && (*m_shader))
		{
			m_shader->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}