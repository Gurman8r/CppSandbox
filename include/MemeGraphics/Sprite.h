#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/ITransformable.h>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API Sprite final
		: public ITrackable
		, public IDrawable
		, public ITransformable
	{
	public:
		Sprite();
		Sprite(Shader* shader, Texture* texture);
		Sprite(const Sprite & copy);
		~Sprite();

		const vec4f &	color() const;
		const Mesh *	mesh() const;
		const Shader *	shader() const;
		const Texture *	texture() const;

		Sprite & color(const vec4f & value);
		Sprite & shader(Shader * value);
		Sprite & texture(Texture * value);

		void draw(RenderTarget & target, RenderState state) const;

		bool isValid() const;
		operator bool() const;

	private:
		Shader * 	m_shader;
		Texture *	m_texture;
		vec4f		m_color;
		Mesh		m_mesh;
	};
}

#endif // !_SPRITE_H_
