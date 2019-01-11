#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/Vertex.h>

namespace ml
{
	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
	{
	public:
		VertexArray();
		VertexArray(uint32_t count);
		VertexArray(const VertexArray & copy);
		~VertexArray();

		VertexArray & clean();
		VertexArray & create(uint32_t count);
		VertexArray & update();
		
		void bind() const;
		void unbind() const;

		inline const uint32_t &	id() const { return m_id; }
		inline const uint32_t &	count() const { return m_id; }

		inline operator bool() const { return (bool)id(); }

	private:
		uint32_t	m_id;
		uint32_t	m_count;
	};

	using VAO = VertexArray;

}

#endif // !_VERTEX_ARRAY_H_
