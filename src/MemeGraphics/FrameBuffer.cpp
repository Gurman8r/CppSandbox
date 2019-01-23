#include <MemeGraphics/FrameBuffer.h>
#include <MemeGraphics/OpenGL.h>

namespace ml
{
	FrameBuffer::FrameBuffer()
		: IHandle()
	{
	}

	FrameBuffer::FrameBuffer(const FrameBuffer & copy)
		: IHandle(copy.m_id)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		clean();
	}


	FrameBuffer & FrameBuffer::clean()
	{
		if (*this)
		{
			OpenGL::deleteFramebuffers(1, (*this));
		}
		return (*this);
	}

	FrameBuffer & FrameBuffer::create()
	{
		if (!(*this) && (m_id = OpenGL::genFramebuffers(1)))
		{

		}
		return (*this);
	}

	
	FrameBuffer & FrameBuffer::bind()
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, (*this));
		return (*this);
	}

	FrameBuffer & FrameBuffer::unbind()
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, NULL);
		return (*this);
	}
}