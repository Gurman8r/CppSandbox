#include <MemeGraphics/Sprite.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/Shapes.hpp>
#include <MemeGraphics/ShaderAPI.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Sprite::Sprite()
		: m_color(Color::White)
		, m_texture(NULL)
		, m_transform()
	{
	}

	Sprite::Sprite(const Sprite & copy)
		: m_color(copy.m_color)
		, m_texture(copy.m_texture)
		, m_transform(copy.m_transform)
	{
	}

	Sprite::~Sprite()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Sprite::dispose()
	{
		return true;
	}

	bool Sprite::loadFromFile(const String & filename)
	{
		return true;
	}

	bool Sprite::loadFromMemory(const Texture * value)
	{
		if (value)
		{
			setTexture(value);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Sprite & Sprite::setColor(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}
	
	Sprite & Sprite::setOrigin(const vec2f & value)
	{
		m_transform.setOrigin(value);
		return (*this);
	}

	Sprite & Sprite::setPosition(const vec2f & value)
	{
		m_transform.setPosition(value);
		return (*this);
	}

	Sprite & Sprite::setRotation(const float value)
	{
		m_transform.setRotation(value);
		return (*this);
	}

	Sprite & Sprite::setScale(const vec2f & value)
	{
		m_transform.setScale(value);
		return (*this);
	}

	Sprite & Sprite::setTexture(const Texture * value)
	{
		m_texture = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Sprite::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_texture)
		{
			if (Uniform * col = batch.mat.uniforms().find(ML_FRAG_MAIN_COL))
				col->data = &m_color;
			
			if (Uniform * tex = batch.mat.uniforms().find(ML_FRAG_MAIN_TEX))
				tex->data = m_texture;

			const ml::vec2f size = (scale() * m_texture->size());
			const ml::vec2f dest = (position() - (size * origin()));

			target.draw(
				ml::Shapes::RectQuad::genSpriteQuad({ dest, size }).data(),
				ml::Shapes::RectQuad::Size,
				batch);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
}