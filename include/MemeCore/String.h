#ifndef _STRING_H_
#define _STRING_H_

#include <MemeCore/IComparable.h>
#include <string>
#include <initializer_list>

namespace ml
{
	template <class _Elem>
	class BasicString
		: public std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem>>
		, public IComparable<BasicString<_Elem>>
		, public IComparable< std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem>>>
	{
	public:
		using data_type		= _Elem;
		using init_type		= std::initializer_list<data_type>;
		using char_traits	= std::char_traits<data_type>;
		using allocator		= std::allocator<data_type>;
		using base_type		= std::basic_string<data_type, char_traits, allocator>;
		using self_type		= BasicString;

	public:
		BasicString()
			: base_type(allocator())
		{
		}
		BasicString(size_t size)
			: base_type(size)
		{
		}
		BasicString(data_type value)
			: BasicString(value, 1)
		{
		}
		BasicString(size_t count, data_type value)
			: base_type(count, value)
		{
		}
		BasicString(const data_type * value)
			: base_type(value)
		{
		}
		BasicString(const allocator & alloc)
			: base_type(alloc)
		{
		}
		BasicString(const base_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		BasicString(const init_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		BasicString(const self_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		template <class Iter>
		BasicString(Iter begin, Iter end, const allocator & alloc = allocator())
			: base_type(begin, end, alloc)
		{
		}
		virtual ~BasicString() {}

		//inline operator bool() const { return !(*this).empty(); }

		inline operator base_type() const 
		{
			return static_cast<base_type>(*this); 
		}

		inline virtual bool equals(const base_type & value) const override
		{
			return ((base_type)(*this) == value);
		}
		inline virtual bool lessThan(const base_type & value) const override
		{
			return ((base_type)(*this) < value);
		}

		inline virtual bool equals(const self_type & value) const override
		{
			return equals((base_type)(value));
		}
		inline virtual bool lessThan(const self_type & value) const override
		{
			return lessThan((base_type)(value));
		}

	private:

	};

	//using String = BasicString<char>;
	//using WString = BasicString<wchar_t>;
	//using String16 = BasicString<char16_t>;
	//using String32 = BasicString<char32_t>;

	//using String = String;
	using String = std::string;

}

#endif // !_STRING_H_
