#ifndef _ML_SPRITE_HPP_
#define _ML_SPRITE_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/RectTransform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Sprite final
		: public ITrackable
		, public IDisposable
		, public IDrawable
		, public IReadable
	{
	public:
		Sprite();
		Sprite(const Sprite & copy);
		~Sprite();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Texture * value);

	public:
		Sprite & setColor	(const vec4f &	value);
		Sprite & setOrigin	(const vec2f &	value);
		Sprite & setPosition(const vec2f &	value);
		Sprite & setRotation(const float	value);
		Sprite & setScale	(const vec2f &	value);
		Sprite & setTexture	(const Texture *value);

	public:
		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const vec4f &	color()		const { return m_color;					}
		inline const vec2f &	origin()	const { return m_transform.origin();	}
		inline const vec2f &	position()	const { return m_transform.position();	}
		inline const float		rotation()	const { return m_transform.rotation();	}
		inline const vec2f &	scale()		const { return m_transform.scale();		}
		inline const Texture *	texture()	const { return m_texture;				}

	private:
		vec4f			m_color;
		const Texture * m_texture;
		RectTransform	m_transform;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPRITE_HPP_