#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Texture.hpp>

namespace ml
{
	class ML_GRAPHICS_API Sprite
		: public ITrackable
		, public IDrawable
		, public IReadable
	{
	public:
		Sprite();
		~Sprite();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
	};
}

#endif // !_SPRITE_HPP_
