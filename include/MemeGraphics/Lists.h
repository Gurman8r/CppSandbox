#ifndef _LISTS_H_
#define _LISTS_H_

#include <MemeCore/List.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Texture.h>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using FloatList		= List<float>;
	using IndexList		= List<uint32_t>;
	using TextureList	= List<const Texture *>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Vertex List : BaseList<Vertex> : std::vector<Vertex>
	class ML_GRAPHICS_API VertexList final
		: public List<Vertex>
	{
	public:
		using base_type = List<data_type>;
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
			if (const size_type imax = ((*this).size() * Vertex::Size))
			{
				if (m_contiguous.size() != imax)
				{
					m_contiguous.resize(imax);
				}
				for (size_type i = 0; i < imax; i++)
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