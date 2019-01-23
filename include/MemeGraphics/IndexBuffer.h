#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <MemeGraphics/GL.h>
#include <MemeGraphics/Lists.h>
#include <MemeGraphics/IHandle.h>

namespace ml
{
	class ML_GRAPHICS_API IndexBuffer final
		: public ITrackable
		, public IHandle
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer & copy);
		~IndexBuffer();

		IndexBuffer & clean();
		IndexBuffer & create(GL::Usage usage, GL::Type type);

		IndexBuffer & bind();
		IndexBuffer & unbind();

		IndexBuffer & bufferData(const uint32_t * data, uint32_t count);
		IndexBuffer & bufferData(const IndexList & data);

		inline const uint32_t *		data()	const { return m_data; }
		inline const uint32_t &		count() const { return m_count; }
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const GL::Type &		type()	const { return m_type; }

	private:
		const uint32_t *	m_data;
		uint32_t			m_count;
		GL::Usage			m_usage;
		GL::Type			m_type;
	};

	using IBO = IndexBuffer;
}

#endif // !_INDEX_BUFFER_H_
