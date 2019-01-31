#ifndef _MESH_H_
#define _MESH_H_

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

	private:
		List<vec3f> m_vp;
		List<vec2f> m_vt;
		List<vec3f> m_vn;
		IndexList	m_vf;
	};
}

#endif // !_MESH_H_
