#ifndef _ML_RENDERER_HPP_
#define _ML_RENDERER_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/RenderStates.hpp>
#include <MemeGraphics/Material.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Renderer final
		: public ITrackable
		, public IDrawable
	{
	public:
		Renderer();

		Renderer(
			const IDrawable		* drawable,
			const RenderStates	& states,
			const Material		& material);

		Renderer(const Renderer & copy);

		virtual ~Renderer();

	public:
		inline const IDrawable		* drawable() const	{ return m_drawable; }
		inline const RenderStates	& states()	 const	{ return m_states; }
		inline const Material		& material() const	{ return m_material; }

	public:
		inline const IDrawable *	& drawable()		{ return m_drawable; }
		inline RenderStates			& states()			{ return m_states; }
		inline Material				& material()		{ return m_material; }

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const IDrawable *	m_drawable;
		RenderStates		m_states;
		Material			m_material;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_