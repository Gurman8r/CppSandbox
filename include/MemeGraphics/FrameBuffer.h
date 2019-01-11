#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include <MemeCore/ITrackable.h>
#include <MemeGraphics/GL.h>
#include <vector>

namespace ml
{
	class ML_GRAPHICS_API FrameBuffer
		: public ITrackable
	{
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer & copy);
		~FrameBuffer();

		FrameBuffer & clean();
		FrameBuffer & create();

		FrameBuffer & bind();
		FrameBuffer & unbind();
		
		FrameBuffer & update(uint32_t width, uint32_t height, GL::Format color, uint32_t depth, uint32_t wrap);

		inline const uint32_t &	id() const { return m_id; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t	m_id;
		uint32_t	m_width;
		uint32_t	m_height;
		GL::Format	m_color;
		uint32_t	m_depth;
		uint32_t	m_wrap;
	};

	using FBO = FrameBuffer;
}

#endif // !_FRAMEBUFFER_H_
