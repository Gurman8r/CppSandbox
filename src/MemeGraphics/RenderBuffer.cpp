#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	RenderBuffer::RenderBuffer()
		: IHandle(NULL)
	{
	}

	RenderBuffer::RenderBuffer(const RenderBuffer & copy)
		: IHandle(copy)
	{
	}

	RenderBuffer::~RenderBuffer()
	{
		clean();
	}
	
	
	RenderBuffer & RenderBuffer::clean()
	{
		if ((*this))
		{
			OpenGL::deleteRenderbuffers(1, (*this));
		}
		return (*this);
	}
	
	RenderBuffer & RenderBuffer::create(int32_t width, int32_t height)
	{
		if (!(*this) && (get_ref() = OpenGL::genRenderbuffers(1)))
		{
			m_width = width;
			m_height = height;
		}
		return (*this);
	}
	
	
	RenderBuffer & RenderBuffer::bind()
	{
		OpenGL::bindRenderbuffer(GL::Renderbuffer, (*this));
		return (*this);
	}
	
	RenderBuffer & RenderBuffer::unbind()
	{
		OpenGL::bindRenderbuffer(GL::Renderbuffer, NULL);
		return (*this);
	}


	RenderBuffer & RenderBuffer::bufferStorage(GL::Format internalFormat)
	{
		OpenGL::renderbufferStorage(GL::Renderbuffer, internalFormat, m_width, m_height);
		return (*this);
	}

	RenderBuffer & RenderBuffer::bufferFramebuffer(GL::FBO_Attachment attachment)
	{
		OpenGL::framebufferRenderbuffer(GL::Framebuffer, attachment, GL::Renderbuffer, (*this));
		return (*this);
	}
}