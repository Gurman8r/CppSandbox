#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <MemeCore/Rect.h>
#include <MemeGraphics/GL.h>
#include <MemeGraphics/Image.h>
#include <MemeGraphics/IHandle.h>

namespace ml
{
	class ML_GRAPHICS_API Texture final
		: public ITrackable
		, public IResource
		, public IHandle
	{
	public:
		enum CoordinateType
		{
			Normalized,	// Texture coordinates in range [0 .. 1]
			Pixels		// Texture coordinates in range [0 .. size]
		};

	public:
		Texture();
		Texture(const Texture & copy);
		~Texture();

		inline Texture & operator=(const Texture & value)
		{
			static Texture temp;
			return swap(temp);
		}

	public:
		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;
		bool loadFromFile(const std::string & filename, const IntRect & area);
		bool loadFromImage(const Image & value);
		bool loadFromImage(const Image & value, const IntRect & area);

		bool update(const Texture& texture);
		bool update(const Image& image);
		bool update(const Image& image, uint32_t x, uint32_t y);
		bool update(const uint8_t* pixels);
		bool update(const uint8_t* pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y);

		bool create(uint32_t width, uint32_t height);
		bool create(uint32_t width, uint32_t height, const vec4f & color);
		bool create(uint32_t width, uint32_t height, const uint8_t * pixels, GL::Format colFmt, GL::Format intFmt, bool smooth, bool repeat);

		Texture & swap(Texture & value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		Texture & setSrgb(bool value);
		Texture & generateMipmap();
		Texture & invalidateMipmap();
		
		const Image copyToImage() const;

	public:
		static void	bind(const Texture * value);

	public:
		inline const vec2u &	size()			const { return m_size; }
		inline const vec2u &	actualSize()	const { return m_actualSize; }
		inline const bool &		isSmooth()		const { return m_isSmooth; }
		inline const bool &		isSrgb()		const { return m_sRgb; }
		inline const bool &		isRepeated()	const { return m_isRepeated; }
		inline const bool &		pixelsFlipped()	const { return m_pixelsFlipped; }
		inline const bool &		hasMipmap()		const { return m_hasMipmap; }

		inline const uint32_t & width()  const { return size()[0]; }
		inline const uint32_t & height() const { return size()[1]; }

	private:
		vec2u	m_size;
		vec2u	m_actualSize;
		bool	m_isSmooth;
		bool	m_sRgb;
		bool	m_isRepeated;
		bool	m_pixelsFlipped;
		bool	m_hasMipmap;
	};
}

#endif // !_TEXTURE_H_
