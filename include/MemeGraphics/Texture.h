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
		Texture();
		Texture(const Texture & copy);
		~Texture();

	public:
		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;
		bool loadFromImage(const Image & value);
		bool loadFromImage(const Image & value, const IntRect & area);

		bool update(const Texture& texture);
		bool update(const Image& image);
		bool update(const uint8_t* pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y);

		bool create(uint32_t width, uint32_t height);
		bool create(uint32_t width, uint32_t height, const vec4f & color);
		bool create(uint32_t width, uint32_t height, const uint8_t * pixels, GL::Format colFmt, GL::Format intFmt, bool smooth, bool repeat, bool mipmapped);

		Texture & swap(Texture & value);
		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		Texture & setSrgb(bool value);
		Texture & generateMipmap();
		Texture & invalidateMipmap();
		
		const Image copyToImage() const;

		inline Texture & operator=(const Texture & value)
		{
			static Texture temp;
			return swap(temp);
		}

	public:
		static void	bind(const Texture * value);

	public:
		inline const vec2u &	size()		const { return m_size; }
		inline const vec2u &	actualSize()const { return m_actualSize; }
		inline const bool &		smooth()	const { return m_smooth; }
		inline const bool &		sRGB()		const { return m_sRGB; }
		inline const bool &		repeated()	const { return m_repeated; }
		inline const bool &		mipmapped()	const { return m_mipmapped; }
		inline const uint32_t & width()		const { return size().front(); }
		inline const uint32_t & height()	const { return size().back(); }

	private:
		vec2u	m_size;
		vec2u	m_actualSize;
		bool	m_smooth;
		bool	m_sRGB;
		bool	m_repeated;
		bool	m_mipmapped;
	};
}

#endif // !_TEXTURE_H_
