#include <MemeGraphics/Texture.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <cassert>
#include <algorithm>
namespace ml
{
	Texture::Texture()
		: m_size(0, 0)
		, m_actualSize(0, 0)
		, m_id(0)
		, m_isSmooth(false)
		, m_sRgb(false)
		, m_isRepeated(false)
		, m_pixelsFlipped(false)
		, m_fboAttachment(false)
		, m_hasMipmap(false)
		, m_cacheID(OpenGL::getUniqueID<Texture>())
	{
	}

	Texture::Texture(const Texture & copy)
		: m_size(0, 0)
		, m_actualSize(0, 0)
		, m_id(0)
		, m_isSmooth(copy.m_isSmooth)
		, m_sRgb(copy.m_sRgb)
		, m_isRepeated(copy.m_isRepeated)
		, m_pixelsFlipped(false)
		, m_fboAttachment(false)
		, m_hasMipmap(false)
		, m_cacheID(OpenGL::getUniqueID<Texture>())
	{
		if (copy.m_id)
		{
			if (create(copy.size()[0], copy.size()[1]))
			{
				update(copy);

				OpenGL::flush();
			}
			else
			{
				Debug::LogError("Failed to copy texture, failed to create new texture");
			}
		}
	}

	Texture::~Texture()
	{
		cleanup();
	}


	bool Texture::cleanup()
	{
		if (m_id)
		{
			OpenGL::deleteTextures(1, &m_id);
			return true;
		}
		return false;
	}

	bool Texture::loadFromFile(const std::string & filename)
	{
		return loadFromFile(filename, IntRect());
	}

	bool Texture::loadFromFile(const std::string & filename, const IntRect & area)
	{
		Image image;
		return image.loadFromFile(filename) && loadFromImage(image);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return loadFromImage(value, IntRect(vec2i::Zero, value.size()));
	}

	bool Texture::loadFromImage(const Image & image, const IntRect & area)
	{
		// Image size
		int width = static_cast<int>(image.size()[0]);
		int height = static_cast<int>(image.size()[1]);

		// Load the entire image if the source area
		// is either empty or contains the whole image
		if ((area.width() == 0 || area.height() == 0) || (
			area.left() <= 0 && 
			area.top() <= 0 && 
			area.width() >= width && 
			area.height() >= height))
		{
			if (create(image.size()[0], image.size()[1]))
			{
				update(image);

				return true;
			}
			return false;
		}
		else
		{
			// Adjust the rectangle to the size of the image
			IntRect rect = area;
			if (rect.left() < 0)		rect.left(0);
			if (rect.top() < 0)			rect.top(0);
			if (rect.right() > width)	rect.right(width);
			if (rect.bottom() > height) rect.bottom(height);

			// Create the texture and upload the pixels
			if (create(rect.width(), rect.height()))
			{
				// Copy the pixels to the texture, row by row
				const uint8_t * pixels = 
					image.pixelsPtr() + 4 * (rect.left() + (width * rect.top()));

				OpenGL::bindTexture(GL::Texture2D, m_id);

				for (int i = 0; i < rect.height(); i++)
				{
					OpenGL::texSubImage2D(
						GL::Texture2D,
						0, 
						0, i, rect.width(), 1, 
						GL::RGBA, 
						GL::UnsignedByte, 
						pixels);

					pixels += 4 * width;
				}

				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMinFilter,
					(m_isSmooth
						? GL::Linear
						: GL::Nearest));

				m_hasMipmap = false;

				// Force an OpenGL flush, so that the texture will appear updated
				OpenGL::flush();

				return true;
			}
			return false;
		}
	}
	
	
	bool Texture::update(const uint8_t * pixels)
	{
		return update(pixels, m_size[0], m_size[1], 0, 0);
	}
	
	bool Texture::update(const uint8_t * pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
	{
		assert(x + width <= m_size[0]);
		assert(y + height <= m_size[1]);

		if (pixels && m_id)
		{
			OpenGL::bindTexture(GL::Texture2D, m_id);

			OpenGL::texSubImage2D(
				GL::Texture2D,
				0,
				x, y, width, height,
				GL::RGBA,
				GL::UnsignedByte,
				pixels);

			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMinFilter,
				(m_isSmooth
					? GL::Linear
					: GL::Nearest));

			m_hasMipmap = false;
			m_pixelsFlipped = false;
			m_cacheID = OpenGL::getUniqueID<Texture>();

			OpenGL::flush();
			return true;
		}
		return false;
	}
	
	bool Texture::update(const Texture & texture)
	{
		return update(texture, 0, 0);
	}
	
	bool Texture::update(const Texture & texture, uint32_t x, uint32_t y)
	{
		//assert(x + texture.m_size[0] <= m_size[0]);
		//assert(y + texture.m_size[1] <= m_size[1]);

		if (!m_id || !texture.m_id)
		{
			return false;
		}

		if (OpenGL::framebuffersAvailable())
		{
			// Save the current bindings so we can restore them after we are done
			int32_t readFramebuffer = OpenGL::getInt(GL::ReadFramebufferBinding);
			int32_t drawFramebuffer = OpenGL::getInt(GL::DrawFramebufferBinding);

			// Create the framebuffers
			uint32_t src = OpenGL::genFramebuffers(1);
			uint32_t dst = OpenGL::genFramebuffers(1);

			if (!src || !dst)
			{
				return Debug::LogError("Cannot copy texture, failed to create a frame buffer object");
			}

			// Link the source texture to the source frame buffer
			OpenGL::bindFramebuffer(GL::FramebufferRead, src);
			OpenGL::framebufferTexture2D(
				GL::FramebufferRead, 
				GL::ColorAttachment0, 
				GL::Texture2D, 
				texture.m_id, 
				0);

			// Link the destination texture to the destination frame buffer
			OpenGL::bindFramebuffer(GL::FramebufferDraw, dst);
			OpenGL::framebufferTexture2D(
				GL::FramebufferDraw, 
				GL::ColorAttachment0,
				GL::Texture2D, 
				m_id, 
				0);

			// A final check, just to be sure...
			uint32_t srcStatus = OpenGL::checkFramebufferStatus(GL::FramebufferRead);
			uint32_t dstStatus = OpenGL::checkFramebufferStatus(GL::FramebufferDraw);

			if ((srcStatus == GL::FramebufferComplete) &&
				(dstStatus == GL::FramebufferComplete))
			{
				// Blit the texture contents from the source to the destination texture
				OpenGL::blitFramebuffer(
					0, 0, texture.m_size[0], texture.m_size[1],
					x, y, x + texture.m_size[0], y + texture.m_size[1],
					GL::ColorBufferBit, 
					GL::Nearest);
			}
			else
			{
				return Debug::LogError("Cannot copy texture, failed to link texture to frame buffer");
			}

			// Restore previously bound framebuffers
			OpenGL::bindFramebuffer(GL::FramebufferRead, readFramebuffer);
			OpenGL::bindFramebuffer(GL::FramebufferDraw, drawFramebuffer);

			// Delete the framebuffers
			OpenGL::deleteFramebuffers(1, &src);
			OpenGL::deleteFramebuffers(1, &dst);

			return true;
		}
		
		return update(texture.copyToImage(), x, y);
	}
	
	bool Texture::update(const Image & image)
	{
		return update(image.pixelsPtr(), image.size()[0], image.size()[1], 0, 0);
	}
	
	bool Texture::update(const Image & image, uint32_t x, uint32_t y)
	{
		return update(image.pixelsPtr(), image.size()[0], image.size()[1], x, y);
	}


	bool Texture::create(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			return Debug::LogError("Failed to create texture, invalid size ( {0}x{1} )",
				width,
				height);
		}

		vec2u actualSize(getValidSize(width), getValidSize(height));

		uint32_t maxSize = OpenGL::getMaxTextureSize();

		if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
		{
			return Debug::LogError("Failed to create texture, its internal size is too high ( {0}x{1} ), maximum is ( {2}x{2} )", 
				actualSize[0],
				actualSize[1], 
				maxSize);
		}

		m_actualSize = actualSize;
		m_size = vec2u(width, height);
		m_pixelsFlipped = false;
		m_fboAttachment = false;

		if (!m_id)
		{
			m_id = OpenGL::genTextures(1);
		}

		if (!m_isRepeated && !OpenGL::edgeClampAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::LogWarning(
					"OpenGL extension SGIS_texture_edge_clamp unavailable\n"
					"Artifacts may occur along texture edges\n"
					"Ensure that hardware acceleration is enabled if available");
				warned = true;
			}
		}

		if (m_sRgb && !OpenGL::textureSrgbAvailable())
		{
			static bool warned = false;
			if (!warned)
			{
				Debug::LogWarning(
					"OpenGL ES extension EXT_sRGB unavailable\n"
					"Automatic sRGB to linear conversion disabled");
				warned = true;
			}
			m_sRgb = false;
		}

		// Initialize the texture
		OpenGL::bindTexture(GL::Texture2D, m_id);
		
		OpenGL::texImage2D(
			GL::Texture2D,
			0,
			(m_sRgb
				? GL::SRGB8_Alpha8
				: GL::RGBA),
			m_actualSize[0], 
			m_actualSize[1], 
			0, 
			GL::RGBA, 
			GL::UnsignedByte, 
			NULL);
		
		OpenGL::texParameter(
			GL::Texture2D,
			GL::TexWrapS,
			(m_isRepeated
				? GL::Repeat
				: (OpenGL::edgeClampAvailable()
					? GL::ClampToEdge
					: GL::Clamp)));
		
		OpenGL::texParameter(
			GL::Texture2D,
			GL::TexWrapT, 
			(m_isRepeated
				? GL::Repeat
				: (OpenGL::edgeClampAvailable()
					? GL::ClampToEdge
					: GL::Clamp)));
		
		OpenGL::texParameter(
			GL::Texture2D, 
			GL::TexMagFilter,
			(m_isSmooth
				? GL::Linear
				: GL::Nearest));
		
		OpenGL::texParameter(
			GL::Texture2D, 
			GL::TexMinFilter, 
			(m_isSmooth
				? GL::Linear
				: GL::Nearest));
		
		m_cacheID = OpenGL::getUniqueID<Texture>();
		m_hasMipmap = false;
		return true;
	}

	bool Texture::create(uint32_t width, uint32_t height, const vec4f & color)
	{
		static Image image;
		image.create(width, height, color);
		return loadFromImage(image);
	}


	Image Texture::copyToImage() const
	{
		// Easy case: empty texture
		if (!m_id)
		{
			return Image();
		}

		// Create an array of pixels
		std::vector<uint8_t> pixels(m_size[0] * m_size[1] * 4);

		if ((m_size == m_actualSize) && !m_pixelsFlipped)
		{
			// Texture is not padded nor flipped, we can use a direct copy
			OpenGL::bindTexture(GL::Texture2D, m_id);
			OpenGL::getTexImage(
				GL::Texture2D, 
				0, 
				GL::RGBA,
				GL::UnsignedByte, 
				&pixels[0]);
		}
		else
		{
			// Texture is either padded or flipped, we have to use a slower algorithm

			// All the pixels will first be copied to a temporary array
			std::vector<uint8_t> allPixels(m_actualSize[0] * m_actualSize[1] * 4);

			OpenGL::bindTexture(GL::Texture2D, m_id);
			
			OpenGL::getTexImage(
				GL::Texture2D,
				0, 
				GL::RGBA, 
				GL::UnsignedByte,
				&allPixels[0]);

			// Then we copy the useful pixels from the temporary array to the final one
			const uint8_t* src = &allPixels[0];
			uint8_t* dst = &pixels[0];
			int srcPitch = m_actualSize[0] * 4;
			int dstPitch = m_size[0] * 4;

			// Handle the case where source pixels are flipped vertically
			if (m_pixelsFlipped)
			{
				src += srcPitch * (m_size[1] - 1);
				srcPitch = -srcPitch;
			}

			for (unsigned int i = 0; i < m_size[1]; ++i)
			{
				std::memcpy(dst, src, dstPitch);
				src += srcPitch;
				dst += dstPitch;
			}
		}

		// Create the image
		Image image;
		image.create(m_size[0], m_size[1], &pixels[0]);
		return image;
	}

	bool Texture::generateMipmap()
	{
		if (m_id)
		{
			if (!OpenGL::framebuffersAvailable())
			{
				OpenGL::bindTexture(GL::Texture2D, m_id);
				
				OpenGL::generateMipmap(GL::Texture2D);
				
				OpenGL::texParameter(
					GL::Texture2D, 
					GL::TexMinFilter,
					(m_isSmooth
						? GL::LinearMipmapLinear
						: GL::NearestMipmapNearest));

				m_hasMipmap = true;

				return true;
			}
		}
		return false;
	}


	void Texture::use() const
	{
		Texture::bind(this);
	}

	void Texture::swap(Texture & other)
	{
		std::swap(m_id,				other.m_id);
		std::swap(m_size,			other.m_size);
		std::swap(m_actualSize,		other.m_actualSize);
		std::swap(m_isSmooth,		other.m_isSmooth);
		std::swap(m_sRgb,			other.m_sRgb);
		std::swap(m_isRepeated,		other.m_isRepeated);
		std::swap(m_pixelsFlipped,	other.m_pixelsFlipped);
		std::swap(m_fboAttachment,	other.m_fboAttachment);
		std::swap(m_hasMipmap,		other.m_hasMipmap);

		m_cacheID = OpenGL::getUniqueID<Texture>();
		other.m_cacheID = OpenGL::getUniqueID<Texture>();
	}


	void Texture::bind(const Texture * value)
	{
		if (value && value->m_id)
		{
			// Bind the texture
			OpenGL::bindTexture(GL::Texture2D, value->m_id);
		}
	}
	
	Texture & Texture::operator=(const Texture & value)
	{
		static Texture temp;
		swap(temp);
		return temp;
	}
	
	
	Texture & Texture::setRepeated(bool value)
	{
		if (value != m_isRepeated)
		{
			m_isRepeated = value;

			if (m_id)
			{
				if (m_isRepeated && !OpenGL::edgeClampAvailable())
				{
					static bool warned = false;
					if (!warned)
					{
						Debug::LogWarning(
							"OpenGL extension SGIS_texture_edge_clamp unavailable\n"
							"Artifacts may occur along texture edges\n"
							"Ensure that hardware acceleration is enabled if available");
						warned = true;
					}
				}

				OpenGL::bindTexture(GL::Texture2D, m_id);
				OpenGL::texParameter(
					GL::Texture2D, 
					GL::TexWrapS, 
					(m_isRepeated
						? GL::Repeat
						: (OpenGL::edgeClampAvailable()
							? GL::ClampToEdge
							: GL::Clamp)));

				OpenGL::texParameter(
					GL::Texture2D, 
					GL::TexWrapT, 
					(m_isRepeated
						? GL::Repeat
						: (OpenGL::edgeClampAvailable()
							? GL::ClampToEdge
							: GL::Clamp)));
			}
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if (m_isSmooth != value)
		{
			m_isSmooth = value;

			if (m_id)
			{
				OpenGL::bindTexture(GL::Texture2D, m_id);

				OpenGL::texParameter(
					GL::Texture2D,
					GL::TexMagFilter,
					(m_isSmooth
						? GL::Linear
						: GL::Nearest));

				if (m_hasMipmap)
				{
					OpenGL::texParameter(
						GL::Texture2D,
						GL::TexMinFilter, 
						(m_isSmooth
							? GL::LinearMipmapLinear
							: GL::LinearMipmapNearest));
				}
				else
				{
					OpenGL::texParameter(
						GL::Texture2D,
						GL::TexMinFilter,
						(m_isSmooth
							? GL::Linear
							: GL::Nearest));
				}
			}
		}
		return (*this);
	}

	Texture & Texture::setSrgb(bool value)
	{
		m_sRgb = value;
		return (*this);
	}


	uint32_t Texture::getValidSize(uint32_t value)
	{
		if (!OpenGL::nonPowerOfTwoAvailable())
		{
			uint32_t powerOfTwo = 1;
			while (powerOfTwo < value)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
		return value;
	}

	void Texture::invalidateMipmap()
	{
		if (m_hasMipmap)
		{
			OpenGL::bindTexture(GL::Texture2D, m_id);
			OpenGL::texParameter(
				GL::Texture2D,
				GL::TexMinFilter,
				(m_isSmooth
					? GL::Linear
					: GL::Nearest));

			m_hasMipmap = false;
		}
	}
}