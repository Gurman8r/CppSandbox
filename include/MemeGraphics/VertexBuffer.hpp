#ifndef _VERTEX_BUFFER_HPP_
#define _VERTEX_BUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	class ML_GRAPHICS_API VertexBuffer final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer & copy);
		~VertexBuffer();

		VertexBuffer & clean();
		VertexBuffer & create(GL::Usage usage);

		VertexBuffer & bind();
		VertexBuffer & unbind();

		VertexBuffer & bufferData(const void * data, uint32_t size);
		VertexBuffer & bufferData(const FloatList & data);

		VertexBuffer & bufferSubData(const void * data, uint32_t size, uint32_t offset);
		VertexBuffer & bufferSubData(const FloatList & data, uint32_t offset);

		inline const void *			data()	const { return m_data;}
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const uint32_t &		size()	const { return m_size; }
		inline const int32_t &		count() const { return m_count; }

	private:
		const void *	m_data;
		GL::Usage		m_usage;
		uint32_t		m_size;		// total length of data
		int32_t			m_count;	// vertex count
	};

	using VBO = VertexBuffer;
}

#endif // !_VERTEX_BUFFER_HPP_
