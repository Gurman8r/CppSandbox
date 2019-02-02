#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <MemeCore/IResource.hpp>
#include <MemeGraphics/Vertex.hpp>
#include <MemeGraphics/Texture.hpp>
#include <MemeGraphics/Lists.hpp>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		Mesh();
		Mesh(const Mesh& copy);
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;

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
