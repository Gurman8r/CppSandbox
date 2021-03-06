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
			const Material		* material,
			const RenderStates	& states);

		Renderer(const Renderer & copy);

		virtual ~Renderer();

	public:
		inline const IDrawable		* drawable() const	{ return m_drawable; }
		inline const Material		* material() const	{ return m_material; }
		inline const RenderStates	& states()	 const	{ return m_states; }

	public:
		inline const IDrawable *	& drawable()		{ return m_drawable; }
		inline const Material *		& material()		{ return m_material; }
		inline RenderStates			& states()			{ return m_states; }

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const IDrawable *	m_drawable;
		const Material *	m_material;
		RenderStates		m_states;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDERER_HPP_