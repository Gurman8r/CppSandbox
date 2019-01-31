#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API FrameBuffer
		: public ITrackable
		, public IHandle<uint32_t>
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
