#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/GL.h>
#include <MemeGraphics/Lists.h>
#include <MemeCore/IHandle.h>

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

#endif // !_VERTEX_ARRAY_H_
