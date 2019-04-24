#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_drawable(NULL)
		, m_states	()
		, m_material()
	{
	}

	Renderer::Renderer(
		const IDrawable		* drawable, 
		const RenderStates	& states, 
		const Material		& material)
		: m_drawable(drawable)
		, m_states	(states)
		, m_material(material)
	{
	}

	Renderer::Renderer(const Renderer & copy)
		: m_drawable(copy.m_drawable)
		, m_states	(copy.m_states)
		, m_material(copy.m_material)
	{
	}

	Renderer::~Renderer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_drawable && m_material.shader())
		{
			m_states.apply();

			m_material.apply();

			target.draw(*m_drawable);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}