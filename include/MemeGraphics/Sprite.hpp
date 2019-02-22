#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Transform.hpp>

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

		Sprite & setTexture(const Texture * value);
		Sprite & setPosition(const vec2f & value);
		Sprite & setScale(const vec2f & value);
		Sprite & setRotation(const float value);
		Sprite & setOrigin(const vec2f & value);
		Sprite & setColor(const vec4f & value);

		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		mutable bool m_requiresUpdate;
		mutable Transform m_transform;

		const Texture * m_texture;
		vec2f			m_position;
		vec2f			m_scale;
		float			m_rotation;
		vec2f			m_origin;
		vec4f			m_color;
	};
}

#endif // !_SPRITE_HPP_
