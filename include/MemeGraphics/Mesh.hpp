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
		inline const List<vec3f> & vertices	()	const { return m_vertices;	}
		inline const List<vec2f> & texcoords()	const { return m_texcoords; }
		inline const List<vec3f> & normals	()	const { return m_normals;	}

	private:
		List<vec3f> m_vertices;
		List<vec2f> m_texcoords;
		List<vec3f> m_normals;
	};
}

#endif // !_MESH_HPP_
