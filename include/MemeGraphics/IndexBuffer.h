#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <MemeGraphics/Export.h>
#include <inttypes.h>

namespace ml
{
	class ML_GRAPHICS_API IndexBuffer
	{
	public:
		IndexBuffer(const uint32_t * data, uint32_t count);
		~IndexBuffer();

		IndexBuffer & clean();
		IndexBuffer & bind();
		IndexBuffer & unbind();

		inline const uint32_t & id() const { return m_id; }
		inline const uint32_t & count() const { return m_count; }

	private:
		uint32_t m_id;
		uint32_t m_count;
	};
}

#endif // !_INDEX_BUFFER_H_
