#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <MemeLib/ITrackable.h>
#include <type_traits>

namespace ml
{
	class ML_API Property final
		: public ITrackable
		, public IComparable<Property>
	{
	public:
		enum Type : int
		{
			None = 0,
			Bool,
			Char,
			Double,
			Float,
			Int,
			String,
			MAX_TYPE
		};

		static const std::string TypeNames[MAX_TYPE];
		
		inline friend std::ostream & operator<<(std::ostream & out, const Property::Type & value)
		{
			return out << Property::TypeNames[value];
		}

		inline friend std::istream & operator>>(std::istream & in, Property::Type & value)
		{
			static int temp;
			in >> temp;
			value = (Property::Type)temp;
			return in;
		}

	public:
		Property();
		Property(const std::string & name);
		Property(const std::string & name, Type type, const std::string & data);
		Property(const std::string & name, bool value);
		Property(const std::string & name, char value);
		Property(const std::string & name, double value);
		Property(const std::string & name, float value);
		Property(const std::string & name, int value);
		Property(const std::string & name, const std::string & value);
		Property(const std::string & name, const char* value);
		Property(const Property & copy);
		~Property();

	public:
		bool 		boolValue() const;
		char 		charValue() const;
		double		doubleValue() const;
		float 		floatValue() const;
		int 		intValue() const;
		std::string stringValue() const;

		Property &	boolValue(bool value);
		Property &	charValue(char value);
		Property &	doubleValue(double value);
		Property &	floatValue(float value);
		Property &	intValue(int value);
		Property &	stringValue(const std::string & value);

	public:
		inline Property & operator=(bool value) { return boolValue(value); }
		inline Property & operator=(char value) { return charValue(value);}
		inline Property & operator=(double value) { return doubleValue(value); }
		inline Property & operator=(float value) { return floatValue(value); }
		inline Property & operator=(int value) { return intValue(value); }
		inline Property & operator=(const std::string & value) { return stringValue(value); }
		inline Property & operator=(const char* value) { return stringValue(value); }

	public:
		inline operator bool()			const { return boolValue(); }
		inline operator char()			const { return charValue(); }
		inline operator double()		const { return doubleValue(); }
		inline operator float()			const { return floatValue(); }
		inline operator int()			const { return intValue(); }
		inline operator std::string()	const { return stringValue(); }
		inline operator const char*()	const { return stringValue().c_str(); }
		inline operator char*()			const { return std::remove_cv_t<char*>((const char*)(*this)); }

		
	public:
		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;
		
		bool equals(const Property & value) const override;
		bool lessThan(const Property & value) const override;

		inline std::string	name() const { return m_name; }
		inline Type			type() const { return m_type; }
		inline std::string	data() const { return m_data; }

	private:
		std::string m_name;
		Type		m_type;
		std::string	m_data;

		inline Property & name(const std::string & value)
		{
			m_name = value;
			return (*this);
		}
		inline Property & type(Type value)
		{
			m_type = value;
			return (*this);
		}
		inline Property & data(const std::string & value)
		{
			m_data = value;
			return (*this);
		}
	};
}

#endif // !_PROPERTY_H_
