#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include <MemeGraphics/IRenderer.hpp>
#include <MemeGraphics/Texture.hpp>

namespace ml
{
	class ML_GRAPHICS_API Sprite
		: public ITrackable
		, public IRenderer
	{
	public:
		Sprite();
		~Sprite();

		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		const Texture * m_texture;
		Vertex	m_vertices[4];
		IntRect m_bounds;
	};
}

#endif // !_SPRITE_HPP_
