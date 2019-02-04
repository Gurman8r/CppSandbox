#ifndef _FRAMEBUFFER_HPP_
#define _FRAMEBUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API FrameBuffer final
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

#endif // !_FRAMEBUFFER_HPP_
