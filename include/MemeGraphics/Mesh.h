#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/GL_Enum.h>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		Mesh();
		Mesh(Enum::Primitive primitive, const VertexList & vertices, const IndexList & indices);
		Mesh(const Mesh& copy);
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		inline const Enum::Primitive & primitive() const { return m_primitive; }
		inline const VertexList & vertices() const { return m_vertices; }
		inline const IndexList & indices() const { return m_indices; }
		inline const FloatList & flattened() const
		{
			if (m_changed)
			{
				m_changed = false;
				return m_flattened = Flatten(vertices());
			}
			return m_flattened;
		}

		static const FloatList & Flatten(const VertexList & value);

	private:
		Enum::Primitive	m_primitive;
		VertexList		m_vertices;
		IndexList		m_indices;

		mutable bool		m_changed;
		mutable FloatList	m_flattened;
	};
}

#endif // !_MESH_H_
