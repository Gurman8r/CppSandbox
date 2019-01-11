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
		return (*this);
	}

	FrameBuffer & FrameBuffer::create()
	{
		return (*this);
	}

	FrameBuffer & FrameBuffer::update()
	{
		return (*this);
	}


	void FrameBuffer::bind() const
	{
	}

	void FrameBuffer::unbind() const
	{
	}
}