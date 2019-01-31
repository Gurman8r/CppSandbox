#include <MemeGraphics/Image.h>
#include <MemeCore/Debug.h>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ml
{
	Image::Image()
		: m_size(vec2u::Zero)
		, m_pixels(Pixels())
	{
	}

	Image::Image(uint32_t width, uint32_t height, const uint8_t * pixels)
		: m_size(width, height)
		, m_pixels(Pixels())
	{
		create(width, height, pixels);
	}

	Image::Image(const Image & copy)
		: m_size(copy.m_size)
		, m_pixels(copy.m_pixels)
	{
	}

	Image::~Image()
	{
	}


	bool Image::cleanup()
	{
		m_pixels.clear();
		return true;
	}
	
	bool Image::loadFromFile(const string & filename)
	{
		stbi_set_flip_vertically_on_load(true);
		
		int32_t width, height, nrChannels;
		if (uint8_t * data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0))
		{
			m_size = { (uint32_t)width, (uint32_t)height };
			
			m_pixels.clear();

			m_pixels.resize(width * height * nrChannels);
			
			memcpy(&m_pixels[0], data, m_pixels.size());

			stbi_image_free(data);
			
			return true;
		}
		return false;
	}
	
	
	Image & Image::create(uint32_t width, uint32_t height, const vec4b & color)
	{
		if (width && height)
		{
			// Create a new pix buffer first for exception safety's sake
			Pixels newPixels(width * height * 4);

			// Fill it with the specified color
			uint8_t* ptr = &newPixels[0];
			uint8_t* end = ptr + newPixels.size();
			while (ptr < end)
			{
				*ptr++ = color[0];
				*ptr++ = color[1];
				*ptr++ = color[2];
				*ptr++ = color[3];
			}

			// Commit the new pix buffer
			m_pixels.swap(newPixels);

			// Assign the new size
			m_size[0] = width;
			m_size[1] = height;
		}
		else
		{
			// Dump the pix buffer
			Pixels().swap(m_pixels);

			// Assign the new size
			m_size[0] = 0;
			m_size[1] = 0;
		}
		return (*this);
	}
	
	Image & Image::create(uint32_t width, uint32_t height, const uint8_t * pixels)
	{
		if (pixels && width && height)
		{
			// Create a new pix buffer first for exception safety's sake
			Pixels newPixels(pixels, pixels + width * height * 4);

			// Commit the new pix buffer
			m_pixels.swap(newPixels);

			// Assign the new size
			m_size[0] = width;
			m_size[1] = height;
		}
		else
		{
			// Dump the pix buffer
			Pixels().swap(m_pixels);

			// Assign the new size
			m_size[0] = 0;
			m_size[1] = 0;
		}
		return (*this);
	}
	
	Image & Image::createMaskFromColor(const vec4b & color, uint8_t alpha)
	{
		// Make sure that the image is not empty
		if (!m_pixels.empty())
		{
			// Replace the alpha of the pixels that match the transparent color
			uint8_t* ptr = &m_pixels[0];
			uint8_t* end = ptr + m_pixels.size();
			while (ptr < end)
			{
				if ((ptr[0] == color[0]) &&
					(ptr[1] == color[1]) &&
					(ptr[2] == color[2]) &&
					(ptr[3] == color[3]))
				{
					ptr[3] = alpha;
				}

				ptr += 4;
			}
		}
		return (*this);
	}

	Image & Image::flipHorizontally()
	{
		if ((*this))
		{
			uint32_t rowSize = m_size[0] * 4;

			for (uint32_t y = 0; y < m_size[1]; ++y)
			{
				Pixels::iterator left = m_pixels.begin() + y * rowSize;
				Pixels::iterator right = m_pixels.begin() + (y + 1) * rowSize - 4;

				for (uint32_t x = 0; x < m_size[0] / 2; ++x)
				{
					std::swap_ranges(left, left + 4, right);
					left += 4;
					right -= 4;
				}
			}
		}
		return (*this);
	}

	Image & Image::flipVertically()
	{
		if ((*this))
		{
			uint32_t rowSize = m_size[0] * 4;

			Pixels::iterator top = m_pixels.begin();
			Pixels::iterator bottom = m_pixels.end() - rowSize;

			for (uint32_t y = 0; y < m_size[1] / 2; ++y)
			{
				std::swap_ranges(top, top + rowSize, bottom);
				top += rowSize;
				bottom -= rowSize;
			}
		}
		return (*this);
	}


	vec4b	Image::getPixel(uint32_t x, uint32_t y) const
	{
		const uint8_t* pix = &m_pixels[(x + y * m_size[0]) * 4];

		return vec4u(pix[0], pix[1], pix[2], pix[3]);
	}

	Image & Image::setPixel(uint32_t x, uint32_t y, const vec4b & color)
	{
		uint8_t* pixel = &m_pixels[(x + y * m_size[0]) * 4];

		*pixel++ = color[0];
		*pixel++ = color[1];
		*pixel++ = color[2];
		*pixel++ = color[3];

		return (*this);
	}
}