#ifndef _FONT_HPP_
#define _FONT_HPP_

#include <MemeGraphics/Glyph.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	class ML_GRAPHICS_API Font final
		: public ITrackable
		, public IResource
	{
	public:
		using GlyphTable = std::map<uint32_t, Glyph>;
		using PageTable  = std::map<uint32_t, GlyphTable>;

		struct Info final : public ITrackable
		{
			String family;

			inline void serialize(std::ostream & out) const override
			{
				out << family;
			}
		};

	public:
		Font();
		Font(const Font & copy);
		~Font();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

		const Glyph & getGlyph(uint32_t value, uint32_t size) const;
		const Info	& getInfo() const;

	public:
		inline void serialize(std::ostream & out) const override
		{
			out << getInfo();
		}

	private:
		mutable PageTable m_pages;
		
		Info  m_info;
		void* m_library;
		void* m_face;
		void* m_stroker;

		Glyph loadGlyph(uint32_t value, uint32_t size) const;
	};
}

#endif // !_FONT_HPP_