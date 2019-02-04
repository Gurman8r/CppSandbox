#ifndef _RENDER_BUFFER_HPP_
#define _RENDER_BUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API RenderBuffer final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		RenderBuffer();
		RenderBuffer(const RenderBuffer & copy);
		~RenderBuffer();

		RenderBuffer & clean();
		RenderBuffer & create(int32_t width, int32_t height);

		RenderBuffer & bind();
		RenderBuffer & unbind();

		RenderBuffer & bufferStorage(GL::Format internalFormat);
		RenderBuffer & bufferFramebuffer(GL::FBO_Attachment attachment);

	public:
		inline const int32_t width()  const { return m_width;  }
		inline const int32_t height() const { return m_height; }

	private:
		int32_t m_width;
		int32_t m_height;
	};

	using RBO = RenderBuffer;
}

#endif // !_RENDER_BUFFER_HPP_
