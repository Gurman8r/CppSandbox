#ifndef _TEXT_H_
#define _TEXT_H_

#include <MemeGraphics/IDrawable.h>
#include <MemeGraphics/ITransformable.h>
#include <MemeGraphics/Font.h>

namespace ml
{
	class ML_GRAPHICS_API Text final
		: public ITrackable
		, public IDrawable
		, public ITransformable
	{
	public:
		Text();
		Text(Shader * shader, Font * font);
		Text(const Text & copy);
		~Text();

		const vec4f &		color() const;
		const Font *		font() const;
		const uint32_t &	fontSize() const;
		const Shader *		shader() const;
		const std::string & string() const;

		Text & color(const vec4f & value);
		Text & font(Font * value);
		Text & fontSize(uint32_t value);
		Text & shader(Shader * value);
		Text & string(const std::string & value);

		void draw(RenderTarget & target, RenderState state) const;

		bool isValid() const;
		operator bool() const;

	private:
		vec4f		m_color;
		Font *		m_font;
		uint32_t	m_fontSize;
		Shader *	m_shader;
		std::string	m_string;

		uint32_t vao, vbo;
	};

	
}

#endif // !_TEXT_H_
