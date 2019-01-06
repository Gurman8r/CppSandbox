#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/VertexArray.h>
#include <MemeGraphics/Primitive.h>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		static const Mesh Triangle;
		static const Mesh Quad;
		static const Mesh Cube;

	public:
		Mesh();
		Mesh(Primitive::Type primitive, const VertexArray& vertices, const IndexArray& indices);
		Mesh(const Mesh& copy);
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		const VertexArray & vertices() const;
		const IndexArray &	indices() const;
		Primitive::Type		primitive() const;

	private:
		VertexArray		m_vertices;
		IndexArray		m_indices;
		Primitive::Type	m_primitive;
	};
}

#endif // !_MESH_H_
