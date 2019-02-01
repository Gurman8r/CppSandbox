#ifndef _ISERIALIZABLE_HPP_
#define _ISERIALIZABLE_HPP_

#include <MemeCore/String.hpp>
#include <MemeCore/TypeInfo.hpp>

namespace ml
{
	class ML_CORE_API ISerializable
	{
	public:
		inline virtual void serialize(std::ostream & out) const
		{
			out << get_type().name;
		}
		inline virtual void deserialize(std::istream & in) 
		{
		}

	public:
		inline TypeInfo get_type() const
		{
			return typeid(*this);
		}

		inline virtual const String::Stream to_stream() const
		{ 
			String::Stream stream;
			stream << (*this);
			return stream;
		}

		inline virtual const String to_str() const
		{
			return to_stream().str();
		}

		inline virtual const char * to_cstr() const
		{
			return to_str().c_str();
		}

	public:
		inline friend std::ostream & operator<<(std::ostream & out, const ISerializable & value)
		{
			value.serialize(out);
			return out;
		}
		inline friend std::istream & operator>>(std::istream & in, ISerializable & value)
		{
			value.deserialize(in);
			return in;
		}
	};
}

#endif // !_ISERIALIZABLE_HPP_
