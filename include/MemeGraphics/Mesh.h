#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Texture.h>
#include <MemeGraphics/Lists.h>

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
		bool loadFromFile(const std::string & filename) override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

		inline const VertexList &	vertices()	const { return m_vertices; }
		inline const IndexList &	indices()	const { return m_indices; }

	private:
		VertexList	m_vertices;
		IndexList	m_indices;

	};
}

#endif // !_MESH_H_
