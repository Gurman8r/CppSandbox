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
		enum { Size = 9 };

	public:
		Vertex();
		Vertex(const vec3f & position);
		Vertex(const vec3f & position, const vec4f & color);
		Vertex(const vec3f & position, const vec2f & texcoords);
		Vertex(const vec3f & position, const vec4f & color, const vec2f & texcoords);
		Vertex(const Vertex& copy);
		~Vertex();

		const vec4f & color() const;
		const vec3f & position() const;
		const vec2f & texcoords() const;

		Vertex & color(const vec4f & value);
		Vertex & position(const vec3f & value);
		Vertex & texcoords(const vec2f & value);

	private:
		vec4f m_color;
		vec3f m_position;
		vec2f m_texcoords;
	};
}

#endif // !_VERTEX_H_