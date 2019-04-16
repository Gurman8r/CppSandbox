#ifndef _ML_SPRITE_HPP_
#define _ML_SPRITE_HPP_

#include <MemeGraphics/IDrawable.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeCore/Transform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Sprite
		: public ITrackable
		, public IDisposable
		, public IDrawable
		, public IReadable
	{
	public:
		Sprite();
		~Sprite();

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const Texture * value);

		Sprite & setTexture(const Texture * value);
		Sprite & setPosition(const vec2f & value);
		Sprite & setScale(const vec2f & value);
		Sprite & setRotation(const float value);
		Sprite & setOrigin(const vec2f & value);
		Sprite & setColor(const vec4f & value);

		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	public:
		inline const Texture *	texture()	const { return m_texture;	}
		inline const vec2f &	position()	const { return m_position;	}
		inline const vec2f &	scale()		const { return m_scale;		}
		inline const float		rotation()	const { return m_rotation;	}
		inline const vec2f &	origin()	const { return m_origin;	}
		inline const vec4f &	color()		const { return m_color;		}

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

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SPRITE_HPP_