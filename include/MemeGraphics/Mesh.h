#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/Enum.h>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		Mesh();
		Mesh(const VertexArray & vertices, const IndexArray & indices);
		Mesh(const Mesh& copy);
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		inline const VertexArray &	vertices() const { return m_vertices; }
		inline const IndexArray &	indices() const { return m_indices; }

	private:
		VertexArray		m_vertices;
		IndexArray		m_indices;
	};
}

#endif // !_MESH_H_
