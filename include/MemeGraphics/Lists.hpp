#ifndef _LISTS_HPP_
#define _LISTS_HPP_

#include <MemeCore/List.hpp>
#include <MemeGraphics/Vertex.hpp>
#include <MemeGraphics/Texture.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using FloatList		= List<float>;
	using IndexList		= List<uint32_t>;
	using TextureList	= List<const Texture *>;

	/* * * * * * * * * * * * * * * * * * * * */

	// Vertex List : List<Vertex> : std::vector<Vertex>
	class ML_GRAPHICS_API VertexList final
		: public List<Vertex>
	{
	public:
		using Base = List<value_type>;
		using Self = VertexList;

	public:
		VertexList()
			: Base()
		{
		}
		VertexList(const Base & value)
			: Base(value)
		{
		}
		VertexList(const Initializer & value)
			: Base(value)
		{
		}
		VertexList(const Self & value)
			: Base(value)
		{
		}
		template <typename Iter>
		VertexList(Iter begin, Iter end)
			: Base(begin, end)
		{
		}
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

#endif // !_LISTS_HPP_