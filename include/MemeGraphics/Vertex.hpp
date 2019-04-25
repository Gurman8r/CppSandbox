#ifndef _ML_VERTEX_HPP_
#define _ML_VERTEX_HPP_

#include <MemeGraphics/Color.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Vector3.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Vertex final
		: public ITrackable
		, public IComparable<Vertex>
	{
	public:
		enum { Size = 9U };

	public:
		using array_type = typename float[Size];

	public:
		Vertex();
		Vertex(const vec3f & position);
		Vertex(const vec3f & position, const vec4f & color);
		Vertex(const vec3f & position, const vec2f & texcoords);
		Vertex(const vec3f & position, const vec4f & color, const vec2f & texcoords);
		Vertex(const Vertex & copy);
		~Vertex();

	public:
		const vec3f & position() const;
		const vec4f & color() const;
		const vec2f & texcoords() const;

	public:
		Vertex & position(const vec3f & value);
		Vertex & color(const vec4f & value);
		Vertex & texcoords(const vec2f & value);

	public:
		Vertex & position(const float x, const float y, const float z);
		Vertex & color(const float r, const float g, const float b, const float a);
		Vertex & texcoords(const float x, const float y);

	public:
		inline const float & operator[](size_t index) const	{ return m_data[index]; }
		inline float &		 operator[](size_t index)		{ return m_data[index]; }

	public:
		inline const float & at(const size_t index) const
		{
			return m_data[index];
		}

		inline const float * ptr() const
		{
			return m_data;
		}

	public:
		inline void serialize(std::ostream & out) const override
		{
			for (size_t i = 0; i < Vertex::Size; i++)
			{
				out << (*this)[i] << ' ';
			}
		}
		
		inline void deserialize(std::istream & in) override
		{
			for (size_t i = 0; i < Size; i++)
			{
				in >> (*this)[i];
			}
		}

	public:
		inline bool equals(const Vertex & other) const override
		{
			for (auto i = 0; i < Size; i++)
			{
				if (at(i) != other.at(i))
				{
					return false;
				}
			}
			return true;
		}

		inline bool lessThan(const Vertex & other) const override
		{
			for (auto i = 0; i < Size; i++)
			{
				if (at(i) >= other.at(i))
				{
					return false;
				}
			}
			return true;
		}

	private:
		array_type m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_VERTEX_HPP_