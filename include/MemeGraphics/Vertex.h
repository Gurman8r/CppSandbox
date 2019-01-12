#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <MemeGraphics/Color.h>
#include <MemeCore/Vector2.h>
#include <MemeCore/Vector3.h>
#include <MemeCore/Vector4.h>

namespace ml
{
	class ML_GRAPHICS_API Vertex final
		: public ITrackable
	{
	public:
		enum { Size = 9U };

	public:
		Vertex();
		Vertex(const vec3f & position);
		Vertex(const vec3f & position, const vec4f & color);
		Vertex(const vec3f & position, const vec2f & texcoords);
		Vertex(const vec3f & position, const vec4f & color, const vec2f & texcoords);
		Vertex(const Vertex& copy);
		~Vertex();

		const vec3f & position() const;
		const vec4f & color() const;
		const vec2f & texcoords() const;

		Vertex & position(const vec3f & value);
		Vertex & color(const vec4f & value);
		Vertex & texcoords(const vec2f & value);

		inline const float * ptr() const { return m_data; }

		inline const float &operator[](std::size_t index) const { return m_data[index]; }
		inline float &		operator[](std::size_t index)		{ return m_data[index]; }

	private:
		float m_data[Size];
	};

	using VertexList	= std::vector<Vertex>;
	using FloatList		= std::vector<float>;
	using IndexList		= std::vector<uint32_t>;
}

#endif // !_VERTEX_H_