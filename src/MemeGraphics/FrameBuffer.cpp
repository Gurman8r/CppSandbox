#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	FrameBuffer::FrameBuffer()
		: m_id(NULL)
	{
	}

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, GL::Format color, uint32_t depth, uint32_t wrap)
	{
		create(width, height, color, depth, wrap);
	}

	FrameBuffer::FrameBuffer(const FrameBuffer & copy)
		: m_id(copy.m_id)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		clean();
	}


	FrameBuffer & FrameBuffer::clean()
	{
		OpenGL::deleteFramebuffers(1, &m_id);
		return (*this);
	}

	FrameBuffer & FrameBuffer::create(uint32_t width, uint32_t height, GL::Format color, uint32_t depth, uint32_t wrap)
	{
		m_width = width;
		m_height = height;
		m_color = color;
		m_depth = depth;
		m_wrap = wrap;
		m_id = OpenGL::genFramebuffers(1);

		bind();

		return update();
	}

	FrameBuffer & FrameBuffer::update()
	{
		return (*this);
	}


	void FrameBuffer::bind() const
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, m_id);
	}

	void FrameBuffer::unbind() const
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, NULL);
	}
}