#ifndef _ML_FONT_HPP_
#define _ML_FONT_HPP_

#include <MemeGraphics/Glyph.hpp>
#include <MemeCore/IReadable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Font final
		: public ITrackable
		, public IReadable
	{
	public:
		using GlyphTable = Map<uint32_t, Glyph>;
		using PageTable  = Map<uint32_t, GlyphTable>;

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
		
		Info	m_info;
		void *	m_library;
		void *	m_face;

		Glyph loadGlyph(uint32_t value, uint32_t size) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FONT_HPP_