#include <MemeGraphics/Font.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

namespace ml
{
	Font::Font()
	{
	}

	Font::Font(const Font & copy)
	{
	}

	Font::~Font()
	{
	}


	bool Font::cleanup()
	{
		// Destroy the font face
		if (m_face)
		{
			FT_Done_Face(static_cast<FT_Face>(m_face));
		}

		// Close the library
		if (m_library)
		{
			FT_Done_FreeType(static_cast<FT_Library>(m_library));
		}

		return true;
	}

	bool Font::loadFromFile(const std::string & filename)
	{
		FT_Library library;
		if (FT_Init_FreeType(&library) != 0)
		{
			Debug::LogError("Failed to load font \"{0}\" (failed to initialize FreeType)", filename);
			return false;
		}
		m_library = library;

		// Load the new font face from the specified file
		FT_Face face;
		if (FT_New_Face(static_cast<FT_Library>(m_library), filename.c_str(), 0, &face) != 0)
		{
			Debug::LogError("Failed to load font \"{0}\" (failed to create the font face)", filename);
			return false;
		}

		// Load the stroker that will be used to outline the font
		FT_Stroker stroker;
		if (FT_Stroker_New(static_cast<FT_Library>(m_library), &stroker) != 0)
		{
			Debug::LogError("Failed to load font \"{0}\" (failed to create the stroker)", filename);
			FT_Done_Face(face);
			return false;
		}

		// Select the unicode character map
		if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
		{
			Debug::LogError("Failed to load font \"{0}\" (failed to set the Unicode character set)", filename);
			FT_Stroker_Done(stroker);
			FT_Done_Face(face);
			return false;
		}

		// Store the loaded font in our ugly void* :)
		m_stroker = stroker;
		m_face = face;

		// Store the font information
		m_info.family = face->family_name ? face->family_name : std::string();

		return true;
	}


	const Glyph & Font::getGlyph(uint32_t c, uint32_t characterSize) const
	{
		// Get the page corresponding to the character size
		GlyphTable& glyphs = m_pages[characterSize].glyphs;

		// Build the key by combining the code point, bold flag, and outline thickness
		const uint32_t key = c;

		// Search the glyph into the cache
		GlyphTable::const_iterator it = glyphs.find(key);
		if (it != glyphs.end())
		{
			// Found: just return it
			return it->second;
		}
		else
		{
			// Not found: we have to load it
			Glyph glyph = loadGlyph(c, characterSize);
			return glyphs.insert(std::make_pair(key, glyph)).first->second;
		}
	}

	const Font::Info & Font::getInfo() const
	{
		return m_info;
	}

	Glyph Font::loadGlyph(uint32_t c, uint32_t characterSize) const
	{
		Glyph glyph;

		FT_Face face = static_cast<FT_Face>(m_face);
		if (!face)
			return glyph;

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, characterSize);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Debug::LogError("FREETYTPE: Failed to load Glyph \'{0}\'", c);
			return glyph;
		}

		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		glyph.texture = texture;

		glyph.position = {
			(int)face->glyph->bitmap_left,
			(int)face->glyph->bitmap_top 
		};

		glyph.size = {
			(int)face->glyph->bitmap.width,
			(int)face->glyph->bitmap.rows
		};

		glyph.advance = uint32_t(face->glyph->advance.x);

		glBindTexture(GL_TEXTURE_2D, 0);

		return glyph;
	}
}