#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	FrameBuffer::FrameBuffer()
		: m_id(NULL)
	{
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

	FrameBuffer & FrameBuffer::create()
	{
		m_id = OpenGL::genFramebuffers(1);
		return (*this);
	}

	
	FrameBuffer & FrameBuffer::bind()
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, m_id);
		return (*this);
	}

	FrameBuffer & FrameBuffer::unbind()
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, NULL);
		return (*this);
	}
}