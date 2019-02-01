#ifndef _BASE_LIST_H_
#define _BASE_LIST_H_

#include <MemeCore/ITrackable.hpp>

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
	public:
		using value_type = _Elem;
		using allocator = _Alloc;
		using size_type = size_t;

		using base_type = std::vector<value_type, allocator>;
		using init_type = std::initializer_list<value_type>;
		using self_type = List<value_type, allocator>;

		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;

	public:
		List()
			: base_type()
		{
		}
		List(const allocator & alloc)
			: base_type(alloc)
		{
		}
		List(const base_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		List(const init_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		List(const self_type & value, const allocator & alloc = allocator())
			: base_type(value, alloc)
		{
		}
		template <class Iter>
		List(Iter begin, Iter end, const allocator & alloc = allocator())
			: base_type(begin, end, alloc)
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
}

#endif // !_BASE_LIST_H_
