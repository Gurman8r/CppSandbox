#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <MemeGraphics/Image.h>
#include <MemeCore/Rect.h>

namespace ml
{
	class RenderTarget;

	class ML_GRAPHICS_API Texture final
		: public ITrackable
		, public IResource
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

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;
		bool loadFromFile(const std::string & filename, const IntRect & area);
		bool loadFromImage(const Image & value);
		bool loadFromImage(const Image & value, const IntRect & area);

		Texture & update(const uint8_t* pixels);
		Texture & update(const uint8_t* pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y);
		Texture & update(const Texture& texture);
		Texture & update(const Texture& texture, uint32_t x, uint32_t y);
		Texture & update(const Image& image);
		Texture & update(const Image& image, uint32_t x, uint32_t y);

		bool create(uint32_t width, uint32_t height);
		bool create(uint32_t width, uint32_t height, const vec4f & color);

		Image	copyToImage() const;
		bool	generateMipmap();
		void	swap(Texture & value);

		static void		bind(const Texture * value);
		static uint32_t	getMaximumSize();

		Texture & operator=(const Texture & value);

	public:
		inline const vec2u &	size()			const { return m_size; }
		inline const vec2u &	actualSize()	const { return m_actualSize; }
		inline const uint32_t & handle()		const { return m_handle; }
		inline const uint64_t & cacheID()		const { return m_cacheID; }
		inline const bool &		isSmooth()		const { return m_isSmooth; }
		inline const bool &		isSrgb()		const { return m_sRgb; }
		inline const bool &		isRepeated()	const { return m_isRepeated; }
		inline const bool &		pixelsFlipped()	const { return m_pixelsFlipped; }
		inline const bool &		fboAttachment()	const { return m_fboAttachment; }
		inline const bool &		hasMipmap()		const { return m_hasMipmap; }

		Texture & setRepeated(bool value);
		Texture & setSmooth(bool value);
		Texture & setSrgb(bool value);

	private:
		static uint32_t getValidSize(uint32_t value);

		void invalidateMipmap();

		vec2u		m_size;
		vec2u		m_actualSize;
		uint32_t	m_handle;
		uint64_t	m_cacheID;
		bool		m_isSmooth;
		bool		m_sRgb;
		bool		m_isRepeated;
		bool		m_pixelsFlipped;
		bool		m_fboAttachment;
		bool		m_hasMipmap;

		friend class RenderTarget;
	};
}

#endif // !_TEXTURE_H_
