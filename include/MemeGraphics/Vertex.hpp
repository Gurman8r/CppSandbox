#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include <MemeGraphics/Color.hpp>
#include <MemeCore/Vector2.hpp>
#include <MemeCore/Vector3.hpp>
#include <MemeCore/Vector4.hpp>

namespace ml
{
	class ML_GRAPHICS_API Vertex final
		: public ITrackable
		, public IComparable<Vertex>
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

		inline const float & operator[](size_t index) const	{ return m_data[index]; }
		inline float &		 operator[](size_t index)		{ return m_data[index]; }

	public:
		inline void serialize(std::ostream & out) const override
		{
			out << "{ ";
			for (uint32_t i = 0; i < Size; i++)
			{
				out << (*this)[i]
					<< ((i < Size - 1) ? ", " : " }");
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
				if ((*this)[i] != other[i])
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
				if ((*this)[i] >= other[i])
				{
					return false;
				}
			}
			return true;
		}

	private:
		float m_data[Size];
	};
}

#endif // !_VERTEX_HPP_