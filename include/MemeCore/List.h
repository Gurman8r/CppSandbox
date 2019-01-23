#ifndef _BASE_LIST_H_
#define _BASE_LIST_H_

#include <MemeCore/ITrackable.h>
#include <MemeCore/IComparable.h>
#include <vector>

namespace ml
{
	// Wrapper for custom std::vector<T, Alloc>
	// Adds serialization & memory tracking
	template <class T, class Alloc = std::allocator<T>>
	class List
		: public std::vector<T, Alloc>
		, public ITrackable
		, public IComparable<std::vector<T, Alloc>>
		, public IComparable<List<T, Alloc>>
	{
	public:
		using size_type = std::size_t;
		using data_type = T;
		using allocator = Alloc;
		using pointer	= data_type * ;
		using reference = data_type & ;

		using base_type = std::vector<data_type, allocator>;
		using init_type = std::initializer_list<data_type>;
		using self_type = List<data_type, allocator>;

	public:
		List() : base_type() {}
		List(const base_type & value) : base_type(value) {}
		List(const init_type & value) : base_type(value) {}
		List(const self_type & value) : base_type(value) {}
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
			for (std::size_t i = 0, imax = (*this).size(); i < imax; i++)
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
