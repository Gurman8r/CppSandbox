#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/GL.h>

namespace ml
{
	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
	{
	public:
		VertexArray();
		VertexArray(const VertexArray & copy);
		~VertexArray();

		VertexArray & clean();
		VertexArray & create(GL::Mode mode);

		VertexArray & bind();
		VertexArray & unbind();

		inline const uint32_t &	id() const { return m_id; }
		inline const GL::Mode & mode() const { return m_mode; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t	m_id;
		GL::Mode	m_mode;
	};

	using VAO = VertexArray;

}

#endif // !_VERTEX_ARRAY_H_
