#include <MemeGraphics/RenderBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	RenderBuffer & RenderBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteRenderbuffers(1, (*this));
			get_ref() = NULL;
		}
		return (*this);
	}
	
	RenderBuffer & RenderBuffer::create(int32_t width, int32_t height)
	{
		if (!(*this) && (get_ref() = ML_GL.genRenderbuffers(1)))
		{
			m_width = width;
			m_height = height;
		}
		return (*this);
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	void RenderBuffer::bind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, (*this));
	}
	
	void RenderBuffer::unbind() const
	{
		ML_GL.bindRenderbuffer(GL::Renderbuffer, NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void RenderBuffer::bufferStorage(GL::Format format) const
	{
		ML_GL.renderbufferStorage(GL::Renderbuffer, format, m_width, m_height);
	}

	void RenderBuffer::setFramebuffer(GL::FBO_Attachment attachment) const
	{
		ML_GL.framebufferRenderbuffer(
			GL::Framebuffer, 
			attachment, 
			GL::Renderbuffer,
			(*this));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}