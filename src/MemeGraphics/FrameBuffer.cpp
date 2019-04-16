#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	FrameBuffer::FrameBuffer()
		: IHandle(NULL)
	{
	}

	FrameBuffer::FrameBuffer(const FrameBuffer & copy)
		: IHandle(copy)
	{
	}

	FrameBuffer::~FrameBuffer()
	{
		clean();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	FrameBuffer & FrameBuffer::clean()
	{
		if ((*this))
		{
			ML_GL.deleteFramebuffers(1, (*this));
			get_ref() = NULL;
		}
		return (*this);
	}

	FrameBuffer & FrameBuffer::create()
	{
		if (!(*this) && (get_ref() = ML_GL.genFramebuffers(1)))
		{
			// good
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	void FrameBuffer::bind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, (*this));
	}

	void FrameBuffer::unbind() const
	{
		ML_GL.bindFramebuffer(GL::Framebuffer, NULL);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void FrameBuffer::setTexture(uint32_t attchment, uint32_t value, GL::Target target, int32_t level) const
	{
		ML_GL.framebufferTexture2D(ml::GL::Framebuffer, attchment, target, value, level);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}