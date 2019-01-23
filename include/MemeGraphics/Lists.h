#ifndef _LISTS_H_
#define _LISTS_H_

#include <MemeCore/BaseList.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Texture.h>

namespace ml
{
	// FloatList : BaseList<float>
	class ML_GRAPHICS_API FloatList final
		: public BaseList<float>
	{
	public:
		using base_type = BaseList<data_type>;
		using self_type = FloatList;
	public:
		FloatList() : base_type() {}
		FloatList(const base_type & value) : base_type(value) {}
		FloatList(const init_type & value) : base_type(value) {}
		FloatList(const self_type & value) : base_type(value) {}
		~FloatList() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// IndexList : BaseList<uint32_t>
	class ML_GRAPHICS_API IndexList final
		: public BaseList<uint32_t>
	{
	public:
		using base_type = BaseList<data_type>;
		using self_type = IndexList;
	public:
		IndexList() : base_type() {}
		IndexList(const base_type & value) : base_type(value) {}
		IndexList(const init_type & value) : base_type(value) {}
		IndexList(const self_type & value) : base_type(value) {}
		~IndexList() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// TextureList : BaseList<const Texture *>
	class ML_GRAPHICS_API TextureList final
		: public BaseList<const Texture *>
	{
	public:
		using base_type = BaseList<data_type>;
		using self_type = TextureList;
	public:
		TextureList() : base_type() {}
		TextureList(const base_type & value) : base_type(value) {}
		TextureList(const init_type & value) : base_type(value) {}
		TextureList(const self_type & value) : base_type(value) {}
		~TextureList() {}
	};

	/* * * * * * * * * * * * * * * * * * * * */

	// VertexList : BaseList<Vertex>
	class ML_GRAPHICS_API VertexList final
		: public BaseList<Vertex>
	{
	public:
		using base_type = BaseList<data_type>;
		using self_type = VertexList;
	public:
		VertexList() : base_type() {}
		VertexList(const base_type & value) : base_type(value) {}
		VertexList(const init_type & value) : base_type(value) {}
		VertexList(const self_type & value) : base_type(value) {}
		~VertexList() {}

	public:
		inline const FloatList & contiguous() const
		{
			if (const std::size_t imax = ((*this).size() * Vertex::Size))
			{
				if (m_contiguous.size() != imax)
				{
					m_contiguous.resize(imax);
				}
				for (std::size_t i = 0; i < imax; i++)
				{
					m_contiguous[i] = (*this)[i / Vertex::Size][i % Vertex::Size];
				}
			}
			else if (!m_contiguous.empty())
			{
				m_contiguous.clear();
			}
			return m_contiguous;
		}

	private:
		mutable FloatList m_contiguous;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_LISTS_H_