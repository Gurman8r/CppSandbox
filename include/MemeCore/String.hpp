#ifndef _ML_STRING_HPP_
#define _ML_STRING_HPP_

#include <MemeCore/IComparable.hpp>
#include <MemeCore/IO.hpp>

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
		, public IComparable<BasicString<_Elem, _Traits, _Alloc>>
	{
	public:
		using Value		= _Elem;
		using Traits	= _Traits;
		using Allocator	= _Alloc;

		using Self		= BasicString<Value, Traits, Allocator>;
		using Base		= std::basic_string<Value, Traits, Allocator>;
		using Stream	= std::basic_stringstream<Value, Traits, Allocator>;
		
		using reference				= typename Base::reference;
		using pointer				= typename Base::pointer;
		using const_reference		= typename Base::const_reference;
		using const_pointer			= typename Base::const_pointer;
		using size_type				= typename Base::size_type;
		using difference_type		= typename Base::difference_type;
		using iterator				= typename Base::iterator;
		using const_iterator		= typename Base::const_iterator;
		using reverse_iterator		= typename Base::reverse_iterator;
		using const_reverse_iterator= typename Base::const_reverse_iterator;
		using _Alty					= typename Base::_Alty;
		using _Alty_traits			= typename Base::_Alty_traits;

	public: // Constructors
		BasicString()
			: Base()
		{
		}

		BasicString(Self && value) noexcept
			: Base(value)
		{
		}

		BasicString(Self && value, const Allocator & alloc)
			: Base(value, alloc)
		{
		}
		
		BasicString(const Self & value)
			: Base(value)
		{
		}
		
		BasicString(const Self & value, const Allocator & alloc)
			: Base(value, alloc)
		{
		}
		
		explicit BasicString(const Allocator & alloc) noexcept
			: Base(alloc)
		{
		}
		
		BasicString(const Self & value, const size_type off, const Allocator & alloc = Allocator())
			: Base(value, off, alloc)
		{
		}
		
		BasicString(const Self & value, const size_type off, const size_type count, const Allocator & alloc = Allocator())
			: Base(value, off, count, alloc)
		{
		}
		
		BasicString(const Value * const value)
			: Base(value)
		{
		}
		
		BasicString(const Value * const value, const size_type count)
			: Base(value, count)
		{
		}
		
		BasicString(const Value * const value, const size_type count, const Allocator & alloc)
			: Base(value, count, alloc)
		{
		}
		
		BasicString(const size_type count, const Value value)
			: Base(count, value)
		{
		}
		
		BasicString(const size_type count, const Value value, const Allocator & alloc)
			: Base(count, value, alloc)
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, std::input_iterator_tag)
			: Base(begin, end, std::input_iterator_tag())
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, std::forward_iterator_tag)
			: Base(begin, end, std::forward_iterator_tag())
		{
		}
		
		template <class Iter>
		BasicString(Iter begin, Iter end, const Allocator & alloc = Allocator())
			: Base(begin, end, alloc)
		{
		}
		
		BasicString(Value * const first, Value * const last, std::random_access_iterator_tag)
			: Base(first, last, std::random_access_iterator_tag())
		{
		}

		BasicString(const Base & value)
			: Base(value)
		{
		}
		
		virtual ~BasicString() noexcept {}


	public: // Assignment Operators
		inline Self & operator=(const Self & other)
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

		inline Self & operator=(Self && other) noexcept
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
		inline operator Base() const
		{
			return static_cast<Base>(*this);
		}

		inline operator const Base &() const
		{
			return static_cast<const Base &>(*this);
		}


	public: // Base Comparison
		inline bool equals(const Base & value) const override
		{
			return value == (const Base &)(*this);
		}
		
		inline bool lessThan(const Base & value) const override
		{
			return value < (const Base &)(*this);
		}


	public: // Self Comparison
		inline bool equals(const Self & value) const override
		{
			return equals((const Base &)(value));
		}
		
		inline bool lessThan(const Self & value) const override
		{
			return lessThan((const Base &)(value));
		}


	public: // Custom
		template<typename T, typename ... A>
		inline static Self Format(Self value, const T & first, const A & ...args)
		{
			return value.format(first, (args)...);
		}

		template<typename T, typename ... A>
		inline Self & format(const T & first, const A & ...args)
		{
			Self::Stream stream;
			stream << first << ml::endl;

			int32_t sink[] = { 0, ((void)(stream << args << ml::endl), 0)... };
			(void)sink;

			for (size_type a = 0; stream.good(); a++)
			{
				const Self fmt = ("{" + std::to_string(a) + "}");

				Self arg;
				if (std::getline(stream, arg))
				{
					for (size_type i = 0;
						(i = (*this).find(fmt, i)) != Self::npos;
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
#endif
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STRING_HPP_
