#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer(
		const IDrawable		* drawable,
		const Shader		* shader,
		const RenderStates	& states,
		const UniformSet	& uniforms)
		: m_drawable(drawable)
		, m_shader(shader)
		, m_states(states)
		, m_uniforms(uniforms)
	{
	}

	Renderer::Renderer() : Renderer(
		NULL, 
		NULL,
		RenderStates(), 
		UniformSet())
	{
	}

	Renderer::Renderer(const Renderer & copy) : Renderer(
		copy.m_drawable, 
		copy.m_shader, 
		copy.m_states, 
		copy.m_uniforms)
	{
	}

	Renderer::~Renderer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_drawable && m_shader)
		{
			m_states.apply();

			m_shader->applyUniforms(m_uniforms);

			m_shader->bind();

			target.draw(*m_drawable);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}