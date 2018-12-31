#ifndef _ISERIALIZABLE_H_
#define _ISERIALIZABLE_H_

#include <MemeLib/Export.h>
#include <string>
#include <type_traits>

namespace ml
{
	class ML_API ISerializable
	{
	public:
		virtual void serialize(std::ostream & out) const
		{
			const std::type_info & type(typeid(*this));
			out << "[" << type.name() << "]";
		}

		virtual void deserialize(std::istream & in) 
		{
		}

		inline friend std::ostream & operator<<(std::ostream & out, const ISerializable & obj)
		{
			obj.serialize(out);
			return out;
		}

		inline friend std::istream & operator>>(std::istream & in, ISerializable & obj)
		{
			obj.deserialize(in);
			return in;
		}
	};
}

#endif // !_ISERIALIZABLE_H_
