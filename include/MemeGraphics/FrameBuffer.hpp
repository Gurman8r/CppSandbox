#ifndef _FRAMEBUFFER_HPP_
#define _FRAMEBUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class Texture;

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

		void bind() const;
		void unbind() const;

		void setTexture(GL::Attachment attchment, const Texture * value) const;
		void setTexture(GL::Attachment attchment, uint32_t value, GL::Target target, int32_t level) const;

	private:

	};

	using FBO = FrameBuffer;
}

#endif // !_FRAMEBUFFER_HPP_
