#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <MemeCore/IResource.h>
#include <MemeCore/Vector2.h>
#include <MemeGraphics/Color.h>
#include <MemeWindow/Icon.h>

namespace ml
{
	class ML_GRAPHICS_API Image final
		: public ITrackable
		, public IResource
	{
	public:
		using Pixels = std::vector<uint8_t>;

	public:
		Image();
		Image(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image(const Image & copy);
		~Image();

		bool cleanup() override;
		bool loadFromFile(const string & filename) override;		

		Image & create(uint32_t width, uint32_t height, const vec4b & color);
		Image & create(uint32_t width, uint32_t height, const uint8_t * pixels);
		Image & createMaskFromColor(const vec4b & color, uint8_t alpha = 0);
		Image & flipHorizontally();
		Image & flipVertically();

		vec4b	getPixel(uint32_t x, uint32_t y) const;
		Image & setPixel(uint32_t x, uint32_t y, const vec4b & color);

		inline const vec2u &	size()		const { return m_size; }
		inline const uint32_t & width()		const { return size()[0]; }
		inline const uint32_t & height()	const { return size()[1]; }
		inline const Pixels &	pixels()	const { return m_pixels; }
		inline const uint8_t *	ptr()		const { return &pixels()[0]; }

		inline const uint8_t &	operator[](uint32_t index) const { return m_pixels[index]; }
		inline uint8_t &		operator[](uint32_t index) { return m_pixels[index]; }

		inline operator bool() const { return !m_pixels.empty(); }
		inline operator Icon() const { return Icon(width(), height(), ptr()); }

	private:
		vec2u	m_size;
		Pixels	m_pixels;
	};
}

#endif // !_IMAGE_H_
