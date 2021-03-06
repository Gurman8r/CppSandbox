#include <MemeGraphics/Renderer.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Renderer::Renderer()
		: m_drawable(NULL)
		, m_material(NULL)
		, m_states	()
	{
	}

	Renderer::Renderer(
		const IDrawable		* drawable,
		const Material		* material,
		const RenderStates	& states)
		: m_drawable(drawable)
		, m_material(material)
		, m_states	(states)
	{
	}

	Renderer::Renderer(const Renderer & copy)
		: m_drawable(copy.m_drawable)
		, m_material(copy.m_material)
		, m_states	(copy.m_states)
	{
	}

	Renderer::~Renderer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Renderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_drawable && m_material)
		{
			m_states.apply();

			if (m_material->bind())
			{
				target.draw(m_drawable);

				m_material->unbind();
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}