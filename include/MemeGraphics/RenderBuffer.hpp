#ifndef _ML_RENDER_BUFFER_HPP_
#define _ML_RENDER_BUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API RenderBuffer final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		RenderBuffer();
		RenderBuffer(const RenderBuffer & copy);
		~RenderBuffer();

	public:
		RenderBuffer & clean();
		RenderBuffer & create(int32_t width, int32_t height);

	public:
		const RenderBuffer & bind() const;
		const RenderBuffer & unbind() const;
		const RenderBuffer & bufferStorage(GL::Format internalFormat) const;
		const RenderBuffer & setFramebuffer(GL::FBO_Attachment attachment) const;

	public:
		inline const int32_t width()  const { return m_width;  }
		inline const int32_t height() const { return m_height; }

	private:
		mutable int32_t m_width;
		mutable int32_t m_height;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using RBO = typename RenderBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_BUFFER_HPP_