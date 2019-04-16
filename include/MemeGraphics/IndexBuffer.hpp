#ifndef _ML_INDEX_BUFFER_HPP_
#define _ML_INDEX_BUFFER_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeGraphics/Lists.hpp>
#include <MemeCore/IHandle.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API IndexBuffer final
		: public ITrackable
		, public IHandle<uint32_t>
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer & copy);
		~IndexBuffer();

		IndexBuffer & clean();
		IndexBuffer & create(GL::Usage usage, GL::Type type);

		void bind() const;
		void unbind() const;

		void bufferData(const uint32_t * data, uint32_t count) const;
		void bufferData(const IndexList & data) const;

		inline const uint32_t *		data()	const { return m_data; }
		inline const uint32_t &		count() const { return m_count; }
		inline const GL::Usage &	usage() const { return m_usage; }
		inline const GL::Type &		type()	const { return m_type; }

	private:
		mutable const uint32_t *m_data;
		mutable uint32_t		m_count;
		mutable GL::Usage		m_usage;
		mutable GL::Type		m_type;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using IBO = IndexBuffer;

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_INDEX_BUFFER_HPP_