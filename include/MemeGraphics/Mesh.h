#ifndef _MESH_H_
#define _MESH_H_

#include <MemeCore/IResource.h>
#include <MemeGraphics/Vertex.h>
#include <MemeGraphics/Texture.h>

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

		Mesh & vertices(const VertexList & value);
		Mesh & indices(const IndexList & value);
		Mesh & textures(const TextureList & value);

		inline const VertexList		&vertices()		const { return m_vertices; }
		inline const IndexList		&indices()		const { return m_indices; }
		inline const TextureList	&textures()		const { return m_textures; }
		inline const FloatList		&contiguous()	const { return update(); }

		const FloatList & update() const;

	private:
		VertexList	m_vertices;
		IndexList	m_indices;
		TextureList m_textures;

		mutable bool		m_changed;
		mutable FloatList	m_contiguous;

		inline Mesh & changed(bool value)
		{
			m_changed = value;
			return (*this);
		}
	};
}

#endif // !_MESH_H_
