#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include <MemeGraphics/Export.h>
#include <MemeCore/ITrackable.h>

namespace ml
{
	class ML_GRAPHICS_API IndexBuffer final
		: public ITrackable
	{
	public:
		enum Usage
		{
			Stream = 0x88E0,
			Dynamic = 0x88E8,
			Static = 0x88E4,
		};


	public:
		IndexBuffer(Usage usage, const uint32_t * data, uint32_t count);
		~IndexBuffer();

		IndexBuffer & clean();
		IndexBuffer & bind();
		IndexBuffer & unbind();

		inline const uint32_t & id() const { return m_id; }
		inline const uint32_t & count() const { return m_count; }
		inline const Usage & usage() const { return m_usage; }

	private:
		uint32_t	m_id;
		uint32_t	m_count;
		Usage		m_usage;
	};
}

#endif // !_INDEX_BUFFER_H_
