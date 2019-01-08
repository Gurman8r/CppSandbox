#ifndef _FONT_H_
#define _FONT_H_

#include <MemeGraphics/Glyph.h>
#include <MemeCore/IResource.h>
#include <map>

namespace ml
{
	class ML_GRAPHICS_API Font final
		: public ITrackable
		, public IResource
	{
	public:
		struct Info final
			: public ITrackable
		{
			std::string family;
		};

	public:
		Font();
		Font(const Font & copy);
		~Font();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		const Glyph &	getGlyph(uint32_t c, uint32_t characterSize) const;
		const Info &	getInfo() const;

	private:
		using GlyphTable = std::map<uint32_t, Glyph>;

		struct Page final
			: public ITrackable
		{
			GlyphTable glyphs;
		};

		using PageTable = std::map<uint32_t, Page>;

		mutable PageTable m_pages;
		Info			  m_info;
		void*			  m_library;
		void*			  m_face;
		void*			  m_stroker;

		Glyph loadGlyph(uint32_t c, uint32_t characterSize) const;
	};
}

#endif // !_FONT_H_
