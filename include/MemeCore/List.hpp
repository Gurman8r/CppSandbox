#ifndef _ML_LIST_HPP_
#define _ML_LIST_HPP_

#include <MemeCore/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// std::vector with serialization
	template <
		class _Elem,
		class _Alloc = std::allocator<_Elem>
	>
	class List
		: public std::vector<_Elem, _Alloc>
		, public ISerializable
		, public IComparable<std::vector<_Elem, _Alloc>>
		, public IComparable<List<_Elem, _Alloc>>
	{
	public:
		using value_type			= _Elem;
		using allocator_type		= _Alloc;
		using self_type				= List<value_type, allocator_type>;
		using base_type				= std::vector<value_type, allocator_type>;
		using initializer_type		= std::initializer_list<value_type>;
		using difference_type		= typename base_type::difference_type;
		using size_type				= typename base_type::size_type;
		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;

	public:
		List()
			: base_type()
		{
		}

		explicit List(const allocator_type & alloc)
			: base_type(alloc)
		{
		}
		
		explicit List(const size_type count, const allocator_type & alloc = allocator_type())
			: base_type(count, alloc)
		{
		}

		List(const base_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		List(const initializer_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		List(const self_type & value, const allocator_type & alloc = allocator_type())
			: base_type(value, alloc)
		{
		}
		
		template <class Iter>
		List(Iter begin, Iter end, const allocator_type & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}
		
		virtual ~List() {}
		
	public:
		inline virtual void serialize(std::ostream & out) const override
		{
			for (size_type i = 0, imax = this->size(); i < imax; i++)
			{
				out << (*this)[i] << " ";
			}
		}
		
		inline virtual void deserialize(std::istream & in) override
		{
		}


	public:
		inline operator base_type() const
		{
			return static_cast<base_type>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}


	public:
		inline virtual bool equals(const base_type & value) const override
		{
			return value == (const base_type &)(*this);
		}
		
		inline virtual bool lessThan(const base_type & value) const override
		{
			return value < (const base_type &)(*this);
		}

		
		inline virtual bool equals(const self_type & value) const override
		{
			return equals((const base_type &)(value));
		}
		
		inline virtual bool lessThan(const self_type & value) const override
		{
			return lessThan((const base_type &)(value));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_LIST_HPP_
