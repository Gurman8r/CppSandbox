#include <MemeGraphics/Sprite.hpp>
#include <MemeGraphics/RenderTarget.hpp>
#include <MemeGraphics/Shapes.hpp>

namespace ml
{
	Sprite::Sprite()
	{
	}

	Sprite::~Sprite()
	{
	}


	bool Sprite::cleanup()
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


	Sprite & Sprite::setTexture(const Texture * value)
	{
		if (m_texture != value)
		{
			m_requiresUpdate = true;
			m_texture = value;
		}
		return (*this);
	}

	Sprite & Sprite::setPosition(const vec2f & value)
	{
		if (m_position != value)
		{
			m_requiresUpdate = true;
			m_position = value;
		}
		return (*this);
	}

	Sprite & Sprite::setScale(const vec2f & value)
	{
		if (m_scale != value)
		{
			m_requiresUpdate = true;
			m_scale = value;
		}
		return (*this);
	}

	Sprite & Sprite::setRotation(const float value)
	{
		if (m_rotation != value)
		{
			m_requiresUpdate = true;
			m_rotation = value;
		}
		return (*this);
	}

	Sprite & Sprite::setOrigin(const vec2f & value)
	{
		if (m_origin != value)
		{
			m_requiresUpdate = true;
			m_origin = value;
		}
		return (*this);
	}

	Sprite & Sprite::setColor(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}

	
	void Sprite::update() const
	{
		if (m_requiresUpdate)
		{
			m_requiresUpdate = false;

			float angle		= m_rotation * Maths::Deg2Rad;
			float cosine	= cosf(angle);
			float sine		= sinf(angle);
			float sxc		= m_scale[0] * cosine;
			float syc		= m_scale[1] * cosine;
			float sxs		= m_scale[0] * sine;
			float sys		= m_scale[1] * sine;
			float tx		= -m_origin[0] * sxc - m_origin[1] * sys + m_position[0];
			float ty		=  m_origin[0] * sxs - m_origin[1] * syc + m_position[1];

			float m00 =  sxc; float m01 = sys; float m02 = tx;
			float m03 = -sxs; float m04 = syc; float m05 = ty;
			float m10 =  0.f; float m11 = 0.f; float m12 = 0.f;

			m_transform.matrix() = {
				m00, m01, 0.f, m02,
				m03, m04, 0.f, m05,
				0.f, 0.f, 1.f, 0.f,
				m10, m11, 0.f, m12,
			};
		}
	}

	void Sprite::draw(RenderTarget & target, RenderBatch batch) const
	{
		update(); // WIP

		if (m_texture)
		{
			if (Uniform * u = batch.uniforms->find("Frag.mainTex")) 
				u->data = m_texture;
			
			if (Uniform * u = batch.uniforms->find("Frag.mainCol")) 
				u->data = &m_color;

			const ml::vec2f size = (m_scale * m_texture->size());
			const ml::vec2f dest = (m_position - (size * m_origin));

			target.draw(
				ml::Shapes::genSpriteQuad({ dest, size }).data(),
				ml::Shapes::RectSize,
				batch);
		}
	}
}