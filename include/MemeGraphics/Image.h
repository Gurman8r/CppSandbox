#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <MemeGraphics/Color.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/IResource.h>

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
		Image(const Image & copy);
		~Image();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;		

		Image & create(std::size_t width, std::size_t height, const vec4u & color);
		Image & create(std::size_t width, std::size_t height, const uint8_t * pixels);
		Image & createMaskFromColor(const vec4u & color, uint8_t alpha = 0);
		Image & flipHorizontally();
		Image & flipVertically();

		vec4u	getPixel(std::size_t x, std::size_t y) const;
		Image & setPixel(std::size_t x, std::size_t y, const vec4u & color);

		inline const vec2s &	size() const { return m_size; }
		inline const Pixels &	pixels() const { return m_pixels; }
		inline const uint8_t *	pixelsPtr() const { return &pixels()[0]; }

		inline const uint8_t &	operator[](std::size_t index) const { return m_pixels[index]; }
		inline uint8_t &		operator[](std::size_t index) { return m_pixels[index]; }

	private:
		vec2s	m_size;
		Pixels	m_pixels;
	};
}

#endif // !_IMAGE_H_
