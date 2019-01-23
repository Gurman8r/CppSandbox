#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <MemeGraphics/GL.h>
#include <MemeGraphics/Lists.h>
#include <MemeGraphics/IHandle.h>

namespace ml
{
	class ML_GRAPHICS_API FrameBuffer
		: public ITrackable
		, public IHandle
	{
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer & copy);
		~FrameBuffer();

		FrameBuffer & clean();
		FrameBuffer & create();

		FrameBuffer & bind();
		FrameBuffer & unbind();

	private:
	};

	using FBO = FrameBuffer;
}

#endif // !_FRAMEBUFFER_H_