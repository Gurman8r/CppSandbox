#include <MemeGraphics/Vertex.hpp>

namespace ml
{
	Vertex::Vertex()
	{
		this->position(vec3f::Zero);
		this->color(Color::White);
		this->texcoords(vec2f::Zero);
	}

	Vertex::Vertex(const vec3f & position)
	{
		this->position(position);
		this->color(Color::White);
		this->texcoords(vec2f::Zero);
	}

	Vertex::Vertex(const vec3f & position, const vec4f & color)
	{
		this->position(position);
		this->color(color);
		this->texcoords(vec2f::Zero);
	}

	Vertex::Vertex(const vec3f & position, const vec2f & texcoords)
	{
		this->position(position);
		this->color(Color::White);
		this->texcoords(texcoords);
	}

	Vertex::Vertex(const vec3f & position, const vec4f & color, const vec2f & texcoords)
	{
		this->position(position);
		this->color(color);
		this->texcoords(texcoords);
	}

	Vertex::Vertex(const Vertex & copy)
	{
		this->position(copy.position());
		this->color(copy.color());
		this->texcoords(copy.texcoords());
	}

	Vertex::~Vertex()
	{
	}
	
	
	const vec3f & Vertex::position() const
	{
		static vec3f temp;
		temp[0] = (*this)[0];
		temp[1] = (*this)[1];
		temp[2] = (*this)[2];
		return temp;
	}
	
	const vec4f & Vertex::color() const
	{
		static vec4f temp;
		temp[0] = (*this)[3];
		temp[1] = (*this)[4];
		temp[2] = (*this)[5];
		temp[3] = (*this)[6];
		return temp;
	}
	
	const vec2f & Vertex::texcoords() const
	{
		static vec2f temp;
		temp[0] = (*this)[7];
		temp[1] = (*this)[8];
		return temp;
	}


	Vertex & Vertex::position(const vec3f & value)
	{
		return position(
			value[0],
			value[1], 
			value[2]);
	}
	
	Vertex & Vertex::color(const vec4f & value)
	{
		return color(
			value[0],
			value[1], 
			value[2], 
			value[3]);
	}
	
	Vertex & Vertex::texcoords(const vec2f & value)
	{
		return texcoords(
			value[0],
			value[1]);
	}


	Vertex & Vertex::position(float x, float y, float z)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		return (*this);
	}

	Vertex & Vertex::color(float r, float g, float b, float a)
	{
		(*this)[3] = r;
		(*this)[4] = g;
		(*this)[5] = b;
		(*this)[6] = a;
		return (*this);
	}

	Vertex & Vertex::texcoords(float x, float y)
	{
		(*this)[7] = x;
		(*this)[8] = y;
		return (*this);
	}
}