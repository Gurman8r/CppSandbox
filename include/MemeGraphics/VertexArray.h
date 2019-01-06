#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Primitive.h>

namespace ml
{
	using FloatArray = std::vector<float>;
	using IndexArray = std::vector<uint32_t>;

	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
	{
	public:
		using vector_type	= std::vector<Vertex>;
		using iterator		= vector_type::iterator;
		using const_iterator= vector_type::const_iterator;

	public:
		VertexArray();
		VertexArray(Primitive::Type primitive);
		VertexArray(const vector_type & values);
		VertexArray(Primitive::Type primitive, const vector_type & values);
		VertexArray(const VertexArray & copy);
		~VertexArray();

		bool			empty() const;
		std::size_t		size() const;
		uint32_t		count() const;
		Primitive::Type	primitive() const;
		const Vertex *	ptr() const;

		VertexArray &	append(const Vertex & value);
		VertexArray &	clear();

		static FloatArray Flatten(const VertexArray & value);

	public:
		inline iterator			begin()
		{
			return m_values.begin();
		};
		inline iterator			end()
		{
			return m_values.end();
		};
		inline const_iterator	begin() const
		{
			return m_values.begin();
		};
		inline const_iterator	end() const
		{
			return m_values.end();
		};
		inline const_iterator	cbegin() const
		{
			return m_values.cbegin();
		};
		inline const_iterator	cend() const
		{
			return m_values.cend();
		};

	public:
		const Vertex & operator[](std::size_t index) const
		{
			return m_values[index];
		}

		Vertex & operator[](std::size_t index)
		{
			return m_values[index];
		}

	private:
		vector_type		m_values;
		Primitive::Type	m_primitive;
	};
}

#endif // !_VERTEX_ARRAY_H_
