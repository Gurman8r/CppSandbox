#ifndef _ML_FRAMEBUFFER_HPP_
#define _ML_FRAMEBUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API FrameBuffer final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		FrameBuffer();
		FrameBuffer(const FrameBuffer & copy);
		~FrameBuffer();

	public:
		FrameBuffer & clean();
		FrameBuffer & create();

	public:
		const FrameBuffer & bind() const;
		const FrameBuffer & unbind() const;
		const FrameBuffer & setTexture(uint32_t attchment, uint32_t value, GL::Target target, int32_t level) const;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using FBO = typename FrameBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FRAMEBUFFER_HPP_