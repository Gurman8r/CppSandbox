#include <MemeGraphics/Texture.h>
#include <MemeGraphics/OpenGL.h>
#include <MemeCore/DebugUtility.h>
#include <cassert>
#include <algorithm>

namespace
{
	inline static uint64_t getUniqueID()
	{
		static uint64_t id = 1;
		return id++;
	}
}

namespace ml
{
	Texture::Texture()
		: m_size(0, 0)
		, m_actualSize(0, 0)
		, m_handle(0)
		, m_isSmooth(false)
		, m_sRgb(false)
		, m_isRepeated(false)
		, m_pixelsFlipped(false)
		, m_fboAttachment(false)
		, m_hasMipmap(false)
		, m_cacheID(getUniqueID())
	{
	}

	Texture::Texture(const Texture & copy)
		: m_size(0, 0)
		, m_actualSize(0, 0)
		, m_handle(0)
		, m_isSmooth(copy.m_isSmooth)
		, m_sRgb(copy.m_sRgb)
		, m_isRepeated(copy.m_isRepeated)
		, m_pixelsFlipped(false)
		, m_fboAttachment(false)
		, m_hasMipmap(false)
		, m_cacheID(getUniqueID())
	{
		if (copy.m_handle)
		{
			if (create(copy.size()[0], copy.size()[1]))
			{
				update(copy);

				glCheck(glFlush());
			}
			else
			{
				Debug::LogError("Failed to copy texture, failed to create new texture");
			}
		}
	}

	Texture::~Texture()
	{
	}


	bool Texture::cleanup()
	{
		if (m_handle)
		{
			glCheck(glDeleteTextures(1, &m_handle));
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
		return image.loadFromFile(filename) && loadFromImage(image, area);
	}

	bool Texture::loadFromImage(const Image & value)
	{
		return loadFromImage(value, IntRect());
	}

	bool Texture::loadFromImage(const Image & image, const IntRect & area)
	{
		// Image size
		int width = static_cast<int>(image.size()[0]);
		int height = static_cast<int>(image.size()[1]);

		// Load the entire image if the source area
		// is either empty or contains the whole image
		if ((area.width() == 0 || area.height() == 0) ||
			(area.left() <= 0 && area.top() <= 0 && area.width() >= width && area.height() >= height))
		{
			if (create(image.size()[0], image.size()[1]))
			{
				update(image);

				return true;
			}
			else
			{
				return false;
			}
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
				const uint8_t * pixels = image.pixelsPtr() + 4 * (rect.left() + (width * rect.top()));

				glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));

				for (int i = 0; i < rect.height(); i++)
				{
					glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rect.width(), 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

					pixels += 4 * width;
				}

				glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

				m_hasMipmap = false;

				// Force an OpenGL flush, so that the texture will appear updated
				// in all contexts immediately (solves problems in multi-threaded apps)
				glCheck(glFlush());

				return true;
			}
			else
			{
				return false;
			}

		}
	}
	
	
	Texture & Texture::update(const uint8_t * pixels)
	{
		return update(pixels, m_size[0], m_size[1], 0, 0);
	}
	
	Texture & Texture::update(const uint8_t * pixels, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
	{
		assert(x + width <= m_size[0]);
		assert(y + height <= m_size[1]);

		if (pixels && m_handle)
		{
			glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
			glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
			glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

			m_hasMipmap = false;
			m_pixelsFlipped = false;
			m_cacheID = getUniqueID();

			glCheck(glFlush());
		}
		return (*this);
	}
	
	Texture & Texture::update(const Texture & texture)
	{
		return update(texture, 0, 0);
	}
	
	Texture & Texture::update(const Texture & texture, uint32_t x, uint32_t y)
	{
		assert(x + texture.m_size[0] <= m_size[0]);
		assert(y + texture.m_size[1] <= m_size[1]);

		if (!m_handle || !texture.m_handle)
		{
			return (*this);
		}

		if (GL_EXT_framebuffer_object && GL_EXT_framebuffer_blit)
		{
			// Save the current bindings so we can restore them after we are done
			GLint readFramebuffer = 0;
			GLint drawFramebuffer = 0;

			glCheck(glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer));
			glCheck(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer));

			// Create the framebuffers
			GLuint sourceFrameBuffer = 0;
			GLuint destFrameBuffer = 0;
			glCheck(glGenFramebuffers(1, &sourceFrameBuffer));
			glCheck(glGenFramebuffers(1, &destFrameBuffer));

			if (!sourceFrameBuffer || !destFrameBuffer)
			{
				Debug::LogError("Cannot copy texture, failed to create a frame buffer object");
				return (*this);
			}

			// Link the source texture to the source frame buffer
			glCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFrameBuffer));
			glCheck(glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_handle, 0));

			// Link the destination texture to the destination frame buffer
			glCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBuffer));
			glCheck(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_handle, 0));

			// A final check, just to be sure...
			GLenum sourceStatus;
			glCheck(sourceStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER));

			GLenum destStatus;
			glCheck(destStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));

			if ((sourceStatus == GL_FRAMEBUFFER_COMPLETE) && (destStatus == GL_FRAMEBUFFER_COMPLETE))
			{
				// Blit the texture contents from the source to the destination texture
				glCheck(glBlitFramebuffer(0, 0, texture.m_size[0], texture.m_size[1], x, y, x + texture.m_size[0], y + texture.m_size[1], GL_COLOR_BUFFER_BIT, GL_NEAREST));
			}
			else
			{
				Debug::LogError("Cannot copy texture, failed to link texture to frame buffer");
			}

			// Restore previously bound framebuffers
			glCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer));
			glCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer));

			// Delete the framebuffers
			glCheck(glDeleteFramebuffers(1, &sourceFrameBuffer));
			glCheck(glDeleteFramebuffers(1, &destFrameBuffer));

			return (*this);
		}
		return update(texture.copyToImage(), x, y);
	}
	
	Texture & Texture::update(const Image & image)
	{
		return update(image.pixelsPtr(), image.size()[0], image.size()[1], 0, 0);
	}
	
	Texture & Texture::update(const Image & image, uint32_t x, uint32_t y)
	{
		return update(image.pixelsPtr(), image.size()[0], image.size()[1], x, y);
	}


	bool Texture::create(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			Debug::LogError("Failed to create texture, invalid size ( {0}x{1} )");
			return false;
		}

		vec2u actualSize(getValidSize(width), getValidSize(height));

		uint32_t maxSize = getMaximumSize();

		if ((actualSize[0] > maxSize) || (actualSize[1] > maxSize))
		{
			Debug::LogError("Failed to create texture, its internal size is too high ( {0}x{1} ), maximum is ( {2}x{2} )", actualSize[0], actualSize[1], maxSize);
			return false;
		}

		m_actualSize = actualSize;
		m_size = vec2u(width, height);
		m_pixelsFlipped = false;
		m_fboAttachment = false;

		if (!m_handle)
		{
			GLuint texture;
			glCheck(glGenTextures(1, &texture));
			m_handle = static_cast<uint32_t>(texture);
		}

		static bool textureEdgeClamp = GL_EXT_texture_edge_clamp || GLEW_EXT_texture_edge_clamp;

		if (!m_isRepeated && !textureEdgeClamp)
		{
			static bool warned = false;

			if (!warned)
			{
				Debug::LogError(
					"OpenGL extension SGIS_texture_edge_clamp unavailable\n"
					"Artifacts may occur along texture edges\n"
					"Ensure that hardware acceleration is enabled if available");
				warned = true;
			}
		}

		static bool textureSrgb = GL_EXT_texture_sRGB;

		if (m_sRgb && !textureSrgb)
		{
			static bool warned = false;

			if (!warned)
			{
				Debug::LogError(
					"OpenGL ES extension EXT_sRGB unavailable\n"
					"Automatic sRGB to linear conversion disabled");
				warned = true;
			}

			m_sRgb = false;
		}

		// Initialize the texture
		glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, (m_sRgb ? GL_SRGB8_ALPHA8 : GL_RGBA), m_actualSize[0], m_actualSize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP)));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP)));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
		m_cacheID = getUniqueID();

		m_hasMipmap = false;

		return true;
	}

	bool Texture::create(uint32_t width, uint32_t height, const vec4f & color)
	{
		return false;
	}


	Image Texture::copyToImage() const
	{
		// Easy case: empty texture
		if (!m_handle)
		{
			return Image();
		}

		// Create an array of pixels
		std::vector<uint8_t> pixels(m_size[0] * m_size[1] * 4);

		if ((m_size == m_actualSize) && !m_pixelsFlipped)
		{
			// Texture is not padded nor flipped, we can use a direct copy
			glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
			glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
		}
		else
		{
			// Texture is either padded or flipped, we have to use a slower algorithm

			// All the pixels will first be copied to a temporary array
			std::vector<uint8_t> allPixels(m_actualSize[0] * m_actualSize[1] * 4);
			glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
			glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

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
		if (m_handle)
		{
			if (!GL_EXT_framebuffer_object)
			{
				glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
				glCheck(glGenerateMipmap(GL_TEXTURE_2D));
				glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));

				m_hasMipmap = true;
			}
		}
		return false;
	}

	void Texture::swap(Texture & other)
	{
		std::swap(m_size,			other.m_size);
		std::swap(m_actualSize,		other.m_actualSize);
		std::swap(m_handle,			other.m_handle);
		std::swap(m_isSmooth,		other.m_isSmooth);
		std::swap(m_sRgb,			other.m_sRgb);
		std::swap(m_isRepeated,		other.m_isRepeated);
		std::swap(m_pixelsFlipped,	other.m_pixelsFlipped);
		std::swap(m_fboAttachment,	other.m_fboAttachment);
		std::swap(m_hasMipmap,		other.m_hasMipmap);

		m_cacheID = getUniqueID();
		other.m_cacheID = getUniqueID();
	}


	void Texture::bind(const Texture * value)
	{
		if (value && value->m_handle)
		{
			// Bind the texture
			glCheck(glBindTexture(GL_TEXTURE_2D, value->m_handle));
		}
	}

	uint32_t Texture::getMaximumSize()
	{
		static bool checked = false;
		static GLint size = 0;

		if (!checked)
		{
			glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));
		}

		return static_cast<uint32_t>(size);
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

			if (m_handle)
			{
				static bool textureEdgeClamp = GL_EXT_texture_edge_clamp || GLEW_EXT_texture_edge_clamp;

				if (m_isRepeated && !textureEdgeClamp)
				{
					static bool warned = false;

					if (!warned)
					{
						Debug::LogError(
							"OpenGL extension SGIS_texture_edge_clamp unavailable\n"
							"Artifacts may occur along texture edges\n"
							"Ensure that hardware acceleration is enabled if available");

						warned = true;
					}
				}

				glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
				glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP)));
				glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP)));
			}
		}
		return (*this);
	}

	Texture & Texture::setSmooth(bool value)
	{
		if (value != m_isSmooth)
		{
			m_isSmooth = value;

			if (m_handle)
			{
				glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
				glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

				if (m_hasMipmap)
				{
					glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST));
				}
				else
				{
					glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));
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
		if (GLEW_ARB_texture_non_power_of_two)
		{
			return value;
		}
		else
		{
			uint32_t powerOfTwo = 1;
			while (powerOfTwo < value)
			{
				powerOfTwo *= 2;
			}
			return powerOfTwo;
		}
	}

	void Texture::invalidateMipmap()
	{
		if (!m_hasMipmap)
		{
			return;
		}

		glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
		glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST));

		m_hasMipmap = false;
	}
}