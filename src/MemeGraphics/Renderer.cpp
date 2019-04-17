#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_drawable(NULL)
		, m_shader	(NULL)
		, m_flags	()
		, m_uniforms()
	{
	}

	Renderer::Renderer(
		const IDrawable		* drawable,
		const Shader		* shader,
		const RenderFlags	& flags,
		const UniformSet	& uniforms)
		: m_drawable		(drawable)
		, m_shader			(shader)
		, m_flags			(flags)
		, m_uniforms		(uniforms)
	{
	}

	Renderer::Renderer(const Renderer & copy)
		: m_drawable(copy.m_drawable)
		, m_shader	(copy.m_shader)
		, m_flags	(copy.m_flags)
		, m_uniforms(copy.m_uniforms)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_drawable && m_shader)
		{
			m_flags.apply();

			m_shader->applyUniforms(m_uniforms);

			m_shader->bind();

			target.draw(*m_drawable);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}