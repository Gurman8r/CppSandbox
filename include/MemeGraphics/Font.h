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
		using GlyphTable = std::map<uint32_t, Glyph>;
		using PageTable  = std::map<uint32_t, GlyphTable>;

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

		const Glyph & getGlyph(uint32_t value, uint32_t size) const;
		const Info	& getInfo() const;

	public:
		inline void serialize(std::ostream & out) const override
		{
			out << getInfo().family;
		}

	private:
		mutable PageTable m_pages;
		Info			  m_info;
		void*			  m_library;
		void*			  m_face;
		void*			  m_stroker;

		Glyph loadGlyph(uint32_t value, uint32_t size) const;
	};
}

#endif // !_FONT_H_
