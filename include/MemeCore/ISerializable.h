#ifndef _ISERIALIZABLE_H_
#define _ISERIALIZABLE_H_

#include <MemeCore/Export.h>
#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>

namespace ml
{
	class ML_CORE_API ISerializable
	{
	public:
		inline virtual void serialize(std::ostream & out) const
		{
			out << "[" << type_info().name() << "]";
		}
		inline virtual void deserialize(std::istream & in) 
		{
		}

	public:
		inline const std::type_info &	type_info()	const { return typeid(*this); }
		inline const std::string		to_string()	const { return to_stream().str(); }
		inline const std::stringstream	to_stream() const { return std::stringstream() << (*this); }

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

#endif // !_ISERIALIZABLE_H_
