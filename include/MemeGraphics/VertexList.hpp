#ifndef _ML_LISTS_HPP_
#define _ML_LISTS_HPP_

#include <MemeGraphics/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API VertexList final
		: public List<Vertex>
	{
	public:
		using base_type = typename List<value_type>;
		using self_type = typename VertexList;

	public:
		VertexList();
		explicit VertexList(const allocator_type & alloc);
		explicit VertexList(const size_type count, const allocator_type & alloc = allocator_type());
		VertexList(const base_type & value);
		VertexList(const init_type & value);
		VertexList(const self_type & value);
		
		template <class Iter>
		VertexList(Iter begin, Iter end)
			: base_type(begin, end)
		{
		}
		
		virtual ~VertexList();

	public:
		const List<float> & contiguous() const;

	private:
		mutable List<float> m_contiguous;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LISTS_HPP_