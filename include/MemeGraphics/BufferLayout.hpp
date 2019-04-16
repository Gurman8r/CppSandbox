#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <MemeGraphics/GL.hpp>
#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API BufferLayout final
		: public ITrackable
	{
	public:
		static const BufferLayout Default;

	public:
		struct Element final
			: public ITrackable
		{
			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalized;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			Element();
			Element(uint32_t index, uint32_t size, GL::Type type, bool normalized, uint32_t stride, uint32_t offset, uint32_t width);
			Element(const Element & copy);

			void use() const;
		};

	public:
		BufferLayout();
		BufferLayout(const std::vector<Element> & elements);
		BufferLayout(const std::initializer_list<Element> & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		void bind() const;

		BufferLayout & push_back(const Element & value);
		
		inline const std::vector<Element> & elements() const { return m_elements; }

	private:
		std::vector<Element> m_elements;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_