#ifndef _BASE_LIST_HPP_
#define _BASE_LIST_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/Check.hpp>

namespace ml
{
	// std::vector with serialization and tracking
	template <
		class _Elem,
		class _Alloc = std::allocator<_Elem>
	>
	class List
		: public std::vector<_Elem, _Alloc>
		, public ITrackable
		, public IComparable<std::vector<_Elem, _Alloc>>
		, public IComparable<List<_Elem, _Alloc>>
	{
		// elements need << operator
		static_assert(
			check::has_leftshift_operator<_Elem>::value,
			"Missing \"operator<<\" | Element must be serializable");

		// elements need >> operator
		static_assert(
			check::has_rightshift_operator<_Elem>::value,
			"Missing \"operator>>\" | Element must be de-serializable");

	public:
		using Value			= _Elem;
		using Allocator		= _Alloc;

		using Base			= std::vector<Value, Allocator>;
		using Self			= List<Value, Allocator>;

		using size_type		= size_t;
		using Initializer	= std::initializer_list<Value>;

		using iterator				= typename Base::iterator;
		using const_iterator		= typename Base::const_iterator;
		using reverse_iterator		= typename Base::reverse_iterator;
		using const_reverse_iterator= typename Base::const_reverse_iterator;

	public:
		List()
			: Base()
		{
		}
		
		List(const Allocator & alloc)
			: Base(alloc)
		{
		}
		
		List(const Base & value, const Allocator & alloc = Allocator())
			: Base(value, alloc)
		{
		}
		
		List(const Initializer & value, const Allocator & alloc = Allocator())
			: Base(value, alloc)
		{
		}
		
		List(const Self & value, const Allocator & alloc = Allocator())
			: Base(value, alloc)
		{
		}
		
		template <class Iter>
		List(Iter begin, Iter end, const Allocator & alloc = Allocator())
			: Base(begin, end, alloc)
		{
		}
		
		virtual ~List() {}
		
	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			out << "{ ";
			for (size_type i = 0, imax = (*this).size(); i < imax; i++)
			{
				out << (*this)[i]
					<< ((i < imax - 1) ? ", " : " }");
			}
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
		}


	public: // Base Cast
		inline operator Base() const
		{
			return static_cast<Base>(*this);
		}

		inline operator const Base &() const
		{
			return static_cast<const Base &>(*this);
		}

	public:
		inline virtual bool equals(const Base & value) const override
		{
			return value == (const Base &)(*this);
		}
		
		inline virtual bool lessThan(const Base & value) const override
		{
			return value < (const Base &)(*this);
		}

		
		inline virtual bool equals(const Self & value) const override
		{
			return equals((const Base &)(value));
		}
		
		inline virtual bool lessThan(const Self & value) const override
		{
			return lessThan((const Base &)(value));
		}
	};
}

#endif // !_BASE_LIST_HPP_
