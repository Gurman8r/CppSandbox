#ifndef _ML_STRING_HPP_
#define _ML_STRING_HPP_

#include <MemeCore/IComparable.hpp>
#include <MemeCore/IO.hpp>
#include <MemeCore/CString.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	template <
		class _Elem,
		class _Traits = std::char_traits<_Elem>,
		class _Alloc = std::allocator<_Elem>
	>
	class BasicString
		: public std::basic_string<_Elem, _Traits, _Alloc>
		, public IComparable<std::basic_string<_Elem, _Traits, _Alloc>>
	{
	public:
		using value_type			= _Elem;
		using traits_type			= _Traits;
		using allocator_type		= _Alloc;
		using self_type				= BasicString<value_type, traits_type, allocator_type>;
		using base_type				= std::basic_string<value_type, traits_type, allocator_type>;
		using Stream				= std::basic_stringstream<value_type, traits_type, allocator_type>;
		using reference				= typename base_type::reference;
		using pointer				= typename base_type::pointer;
		using const_reference		= typename base_type::const_reference;
		using const_pointer			= typename base_type::const_pointer;
		using size_type				= typename base_type::size_type;
		using difference_type		= typename base_type::difference_type;
		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;
		using _Alty					= typename base_type::_Alty;
		using _Alty_traits			= typename base_type::_Alty_traits;

	public: // Constructors
		BasicString()
			: base_type()
		{
		}

		BasicString(self_type && value) noexcept
			: base_type(value)
		{
		}

		BasicString(self_type && value, const allocator_type & alloc)
			: base_type(value, alloc)
		{
		}
		
		BasicString(const self_type & value)
			: base_type(value)
		{
		}
		
		BasicString(const self_type & value, const allocator_type & alloc)
			: base_type(value, alloc)
		{
		}
		
		explicit BasicString(const allocator_type & alloc) noexcept
			: base_type(alloc)
		{
		}
		
		BasicString(const self_type & value, const size_type off, const allocator_type & alloc = allocator_type())
			: base_type(value, off, alloc)
		{
		}
		
		BasicString(const self_type & value, const size_type off, const size_type count, const allocator_type & alloc = allocator_type())
			: base_type(value, off, count, alloc)
		{
		}
		
		BasicString(const value_type * const value)
			: base_type(value)
		{
		}
		
		BasicString(const value_type * const value, const size_type count)
			: base_type(value, count)
		{
		}
		
		BasicString(const value_type * const value, const size_type count, const allocator_type & alloc)
			: base_type(value, count, alloc)
		{
		}
		
		BasicString(const size_type count, const value_type value)
			: base_type(count, value)
		{
		}
		
		BasicString(const size_type count, const value_type value, const allocator_type & alloc)
			: base_type(count, value, alloc)
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, std::input_iterator_tag)
			: base_type(begin, end, std::input_iterator_tag())
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, std::forward_iterator_tag)
			: base_type(begin, end, std::forward_iterator_tag())
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, const allocator_type & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}
		
		BasicString(value_type * const first, value_type * const last, std::random_access_iterator_tag)
			: base_type(first, last, std::random_access_iterator_tag())
		{
		}

		BasicString(const base_type & value)
			: base_type(value)
		{
		}
		
		virtual ~BasicString() noexcept {}


	public: // Assignment Operators
		inline self_type & operator=(const self_type & other)
		{
			using namespace std;
			if (this != _STD addressof(other))
			{
#pragma warning(push)
#pragma warning(disable: 4127)
				if (_Alty_traits::propagate_on_container_copy_assignment::value
					&& this->_Getal() != other._Getal())
				{
					this->_Tidy_deallocate();
				}
#pragma warning(pop)
				this->_Copy_alloc(other._Getal());

				auto & otherData = other._Get_data();

				this->assign(otherData._Myptr(), otherData._Mysize);
			}
			return (*this);
		}

		inline self_type & operator=(self_type && other) noexcept
		{
			using namespace std;
			if (this != _STD addressof(other))
			{
				this->_Tidy_deallocate();
				
				this->_Move_alloc(other._Getal());
				
				this->_Assign_rv_contents(
					_STD move(other),
					bool_constant<_Always_equal_after_move<_Alty>>{});
			}
			return (*this);
		}
	

	public: // Cast Operators
		inline operator base_type() const
		{
			return static_cast<base_type>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}


	public: // base_type Comparison
		inline bool equals(const base_type & value) const override
		{
			return value == (const base_type &)(*this);
		}
		
		inline bool lessThan(const base_type & value) const override
		{
			return value < (const base_type &)(*this);
		}


	public: // Custom
		template <class T, typename ... A>
		inline static self_type Format(self_type value, const T & first, const A & ...args)
		{
			return value.format(first, (args)...);
		}

		template <class T, typename ... A>
		inline self_type & format(const T & arg0, const A & ...args)
		{
			self_type::Stream ss;
			ss << arg0 << ml::endl;

			int32_t sink[] = { 0, ((void)(ss << args << ml::endl), 0)... };
			(void)sink;

			for (size_type a = 0; ss.good(); a++)
			{
				const self_type fmt = ("{" + std::to_string(a) + "}");

				self_type arg;
				if (std::getline(ss, arg))
				{
					for (size_type i = 0;
						(i = (*this).find(fmt, i)) != self_type::npos;
						(i += arg.size()))
					{
						(*this).replace(i, fmt.size(), arg);
					}
				}
			}
			return (*this);
		}
		
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <
		class _Elem,
		class _Traits,
		class _Alloc
	>
	struct hash<ml::BasicString<_Elem, _Traits, _Alloc>>
	{
		using argument_type = ml::BasicString<_Elem, _Traits, _Alloc>;
		using result_type	= size_t;

		inline result_type operator()(const argument_type & value) const noexcept
		{
			return _Hash_array_representation(value.c_str(), value.size());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
#ifdef ML_STD_STRING
	using String	= std::basic_string<char>;
	using WString	= std::basic_string<wchar_t>;
	using U16string = std::basic_string<char16_t>;
	using U32string	= std::basic_string<char32_t>;
#else
	using String	= BasicString<char>;
	using Wstring	= BasicString<wchar_t>;
	using U16string = BasicString<char16_t>;
	using U32string	= BasicString<char32_t>;

	using SStream	= String::Stream;
#endif
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STRING_HPP_
