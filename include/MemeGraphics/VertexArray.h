#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Enum.h>

namespace ml
{
	using Vertices		= std::vector<Vertex>;
	using FloatArray	= std::vector<float>;
	using IndexArray	= std::vector<uint32_t>;

	class ML_GRAPHICS_API VertexArray final
		: public ITrackable
	{
	public:
		VertexArray();
		VertexArray(Enum::Primitive primitive);
		VertexArray(const Vertices & values);
		VertexArray(Enum::Primitive primitive, const Vertices & values);
		VertexArray(const VertexArray & copy);
		~VertexArray();

		bool			empty() const;
		std::size_t		size() const;
		uint32_t		count() const;
		Enum::Primitive	primitive() const;
		const Vertex *	ptr() const;

		VertexArray &	append(const Vertex & value);
		VertexArray &	clear();

	public:
		const Vertex & operator[](std::size_t index) const
		{
			return m_values[index];
		}

		Vertex & operator[](std::size_t index)
		{
			return m_values[index];
		}
		
	public:
		static const FloatArray & Flatten(const VertexArray & value);
		static void GenVAO(uint32_t count, uint32_t & vao);

	private:
		uint32_t		m_id;
		Enum::Primitive	m_primitive;
		Vertices		m_values;
	};

}

#endif // !_VERTEX_ARRAY_H_
