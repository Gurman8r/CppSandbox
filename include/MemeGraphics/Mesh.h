#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/GL.h>

namespace ml
{
	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IResource
	{
	public:
		Mesh();
		Mesh(GL::Mode mode, const VertexList & vertices, const IndexList & indices);
		Mesh(const Mesh& copy);
		~Mesh();

		bool cleanup() override;
		bool loadFromFile(const std::string & filename) override;

		const GL::Mode &	mode() const;
		const VertexList &	vertices() const;
		const IndexList &	indices() const;
		const FloatList &	flattened() const;

		void update() const;

		Mesh & mode(GL::Mode value);
		Mesh & vertices(const VertexList & value);
		Mesh & indices(const IndexList & value);

	private:
		GL::Mode	m_mode;
		VertexList	m_vertices;
		IndexList	m_indices;

		mutable bool		m_changed;
		mutable FloatList	m_flattened;

		inline Mesh & changed(bool value) { m_changed = value; return (*this); }
	};
}

#endif // !_MESH_H_
