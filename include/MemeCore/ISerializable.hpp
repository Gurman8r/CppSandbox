#ifndef _ML_I_SERIALIZABLE_HPP_
#define _ML_I_SERIALIZABLE_HPP_

#include <MemeCore/IO.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API ISerializable
	{
	public: // Operators
		/* * * * * * * * * * * * * * * * * * * * */
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

	public: // Virtual
		/* * * * * * * * * * * * * * * * * * * * */
		inline virtual void serialize(std::ostream & out) const
		{
			out << GetTypeInfo().name();
		}
		
		inline virtual void deserialize(std::istream & in)
		{
		}

	public: // Functions
		/* * * * * * * * * * * * * * * * * * * * */
		inline const std::type_info & GetTypeInfo() const
		{
			return typeid(*this);
		}

		inline CString ToCString() const
		{
			return ToString().c_str();
		}

		inline String ToString() const
		{
			return ToStream().str();
		}

		inline SStream ToStream() const
		{ 
			SStream ss;
			ss << (*this);
			return ss;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_SERIALIZABLE_HPP_