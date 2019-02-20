#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <MemeCore/IReadable.hpp>
#include <MemeGraphics/Vertex.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IReadable
	{
	public:
		Mesh();
		Mesh(const VertexList & vertices);
		Mesh(const VertexList & vertices, const IndexList & indices);
		Mesh(const Mesh & copy);
		~Mesh();

	public:
		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const IndexList & indices);

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const VertexList &	vertices	() const { return m_vertices;	}
		inline const IndexList &	indices		() const { return m_indices;	}
		inline const FloatList &	contiguous	() const { return m_contiguous; }

	private:
		VertexList	m_vertices;
		IndexList	m_indices;
		FloatList	m_contiguous;
	};
}

#endif // !_MESH_HPP_
