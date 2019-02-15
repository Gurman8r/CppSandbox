#ifndef _LISTS_HPP_
#define _LISTS_HPP_

#include <MemeCore/List.hpp>
#include <MemeGraphics/Vertex.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using FloatList		= List<float>;
	using IndexList		= List<uint32_t>;
	using TextureList	= List<const Texture *>;
	using UniformList	= List<Uniform>;

	/* * * * * * * * * * * * * * * * * * * * */

	// VertexList | List<Vertex> | std::vector<Vertex>
	class ML_GRAPHICS_API VertexList final
		: public List<Vertex>
	{
	public:
		using base_type = List<value_type>;
		using self_type = VertexList;

	public:
		VertexList();
		explicit VertexList(const allocator_type & alloc);
		explicit VertexList(const size_type count, const allocator_type & alloc = allocator_type());
		VertexList(const base_type & value);
		VertexList(const initializer_type & value);
		VertexList(const self_type & value);
		
		template <class Iter>
		VertexList(Iter begin, Iter end)
			: base_type(begin, end)
		{
		}
		
		virtual ~VertexList();

	public:
		const FloatList & contiguous() const;

	private:
		mutable FloatList m_contiguous;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_LISTS_HPP_