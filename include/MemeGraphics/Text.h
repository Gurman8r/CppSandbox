#ifndef _TEXT_H_
#define _TEXT_H_

#include <MemeGraphics/IRenderer.h>
#include <MemeGraphics/Font.h>
#include <MemeGraphics/RenderTarget.h>
#include <MemeGraphics/Lists.h>

namespace ml
{
	class ML_GRAPHICS_API Text final
		: public ITrackable
		, public IRenderer
	{
	public:
		Text();
		Text(const Text & copy);
		~Text();

	public:
		Text & setPosition(const vec2f & value);
		Text & setScale(const vec2f & value);
		Text & setColor(const vec4f & value);
		Text & setFont(const Font * value);
		Text & setFontSize(uint32_t value);
		Text & setText(const std::string & value);

	public:
		void update() const;
		void draw(RenderTarget & target, RenderBatch batch) const override;

	private:
		mutable bool		m_requiresUpdate;
		mutable TextureList	m_textures;
		mutable std::vector<FloatList> m_vertices;

		const Font* m_font;
		uint32_t	m_fontSize;
		vec2f		m_position;
		vec2f		m_scale;
		std::string m_text;
		vec4f		m_color;
	};
}
#endif // !_TEXT_H_
