#ifndef _I_SERIALIZABLE_HPP_
#define _I_SERIALIZABLE_HPP_

#include <MemeCore/Export.hpp>
#include <MemeCore/String.hpp>

namespace ml
{
	class ML_CORE_API ISerializable
	{
	public:
		inline virtual void serialize(std::ostream & out) const
		{
			out << get_type().name();
		}
		
		inline virtual void deserialize(std::istream & in) 
		{
		}

	public:
		inline const std::type_info & get_type() const
		{
			return typeid(*this);
		}

		inline CString to_cstr() const
		{
			return to_str().c_str();
		}

		inline String to_str() const
		{
			return to_stream().str();
		}

		inline SStream to_stream() const
		{ 
			SStream ss;
			ss << (*this);
			return ss;
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

#endif // !_I_SERIALIZABLE_HPP_
