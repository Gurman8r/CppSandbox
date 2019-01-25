#ifndef _BASE_LIST_H_
#define _BASE_LIST_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IComparable.h>
#include <vector>

namespace ml
{
	// Wrapper for std::vector. Adds serialization and tracking.
	template <class _T, class _Alloc = std::allocator<_T>>
	class List
		: public std::vector<_T, _Alloc>
		, public ITrackable
		, public IComparable<std::vector<_T, _Alloc>>
		, public IComparable<List<_T, _Alloc>>
	{
	public:
		using size_type = size_t;
		using data_type = _T;
		using Allocator = _Alloc;

		using base_type = std::vector<data_type, Allocator>;
		using init_type = std::initializer_list<data_type>;
		using self_type = List<data_type, Allocator>;

		using iterator				= base_type::iterator;
		using const_iterator		= base_type::const_iterator;
		using reverse_iterator		= base_type::reverse_iterator;
		using const_reverse_iterator= base_type::const_reverse_iterator;

	public:
		List()
			: base_type(Allocator())
		{
		}
		List(const Allocator & alloc)
			: base_type(alloc)
		{
		}
		List(const base_type & value, const Allocator & alloc = Allocator())
			: base_type(value, alloc)
		{
		}
		List(const init_type & value, const Allocator & alloc = Allocator())
			: base_type(value, alloc)
		{
		}
		List(const self_type & value, const Allocator & alloc = Allocator())
			: base_type(value, alloc)
		{
		}
		template <class Iter>
		List(Iter begin, Iter end, const Allocator & alloc = Allocator())
			: base_type(begin, end, alloc)
		{
		}
		virtual ~List() {}
		
	public:
		inline operator base_type() const
		{
			return static_cast<base_type>(*this);
		}

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

	public:
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
	};
}

#endif // !_BASE_LIST_H_
