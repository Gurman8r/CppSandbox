#include <MemeGraphics/Vertex.h>

namespace ml
{
	Vertex::Vertex()
		: m_position(vec3f::Zero)
		, m_color(Color::White)
		, m_texcoords(vec2f::Zero)
	{
	}

	Vertex::Vertex(const vec3f & position)
		: m_position(position)
		, m_color(Color::White)
		, m_texcoords(vec2f::Zero)
	{
	}

	Vertex::Vertex(const vec3f & position, const vec4f & color)
		: m_position(position)
		, m_color(color)
		, m_texcoords(vec2f::Zero)
	{
	}

	Vertex::Vertex(const vec3f & position, const vec2f & texcoords)
		: m_position(position)
		, m_color(Color::White)
		, m_texcoords(texcoords)
	{
	}

	Vertex::Vertex(const vec3f & position, const vec4f & color, const vec2f & texcoords)
		: m_position(position)
		, m_color(color)
		, m_texcoords(texcoords)
	{
	}

	Vertex::Vertex(const Vertex & copy)
		: m_color(copy.m_color)
		, m_position(copy.m_position)
		, m_texcoords(copy.m_texcoords)
	{
	}

	Vertex::~Vertex()
	{
	}
	
	
	const vec4f & Vertex::color() const
	{
		return m_color;
	}
	
	const vec3f & Vertex::position() const
	{
		return m_position;
	}
	
	const vec2f & Vertex::texcoords() const
	{
		return m_texcoords;
	}


	Vertex & Vertex::color(const vec4f & value)
	{
		m_color = value;
		return (*this);
	}
	
	Vertex & Vertex::position(const vec3f & value)
	{
		m_position = value;
		return (*this);
	}
	
	Vertex & Vertex::texcoords(const vec2f & value)
	{
		m_texcoords = value;
		return (*this);
	}
}