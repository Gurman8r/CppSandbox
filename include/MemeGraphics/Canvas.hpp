#ifndef _CANVAS_HPP_
#define _CANVAS_HPP_

#include <MemeGraphics/Text.hpp>
#include <MemeGraphics/RenderBatch.hpp>

namespace ml
{
	class ML_GRAPHICS_API Canvas
		: public ITrackable
		, public INonCopyable
	{
	public:
		Canvas();
		~Canvas();

	private:
		mutable VAO m_vao;
		mutable VBO m_vbo;
	};
}

#endif // !_CANVAS_HPP_
