#include <MemeGraphics/FrameBuffer.hpp>
#include <MemeGraphics/OpenGL.hpp>
#include <MemeGraphics/Texture.hpp>

namespace ml
{
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


	FrameBuffer & FrameBuffer::clean()
	{
		if ((*this))
		{
			OpenGL::deleteFramebuffers(1, (*this));
		}
		return (*this);
	}

	FrameBuffer & FrameBuffer::create()
	{
		if (!(*this) && (get_ref() = OpenGL::genFramebuffers(1)))
		{
			// good
		}
		return (*this);
	}

	
	void FrameBuffer::bind() const
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, (*this));
	}

	void FrameBuffer::unbind() const
	{
		OpenGL::bindFramebuffer(GL::Framebuffer, NULL);
	}

	void FrameBuffer::setTexture(const Texture * value, GL::Attachment attchment) const
	{
		setTexture((*value), attchment, value->target(), value->level());
	}

	void FrameBuffer::setTexture(uint32_t value, GL::Attachment attchment, GL::Target target, int32_t level) const
	{
		OpenGL::framebufferTexture2D(ml::GL::Framebuffer, attchment, target, value, level);
	}
}