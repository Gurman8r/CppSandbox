#ifndef _VERTEX_ARRAY_HPP_
#define _VERTEX_ARRAY_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		VertexArray();
		VertexArray(const VertexArray & copy);
		~VertexArray();

		VertexArray & clean();
		VertexArray & create(GL::Mode mode);

		VertexArray & bind();
		VertexArray & unbind();

		inline const GL::Mode & mode()	const { return m_mode; }

	private:
		GL::Mode	m_mode;
	};

	using VAO = VertexArray;

}

#endif // !_VERTEX_ARRAY_HPP_
