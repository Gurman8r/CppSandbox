#ifndef _ARGS_HPP_
#define _ARGS_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeScript/Export.hpp>

namespace ml
{
	// Wrapper for std::vector<String>
	class ML_SCRIPT_API Args final
		: public ITrackable
		, public IComparable<Args>
	{
	public:
		using Value			= String;
		using vector_type			= std::vector<Value>;
		using iterator				= vector_type::iterator;
		using const_iterator		= vector_type::const_iterator;
		using reverse_iterator		= vector_type::reverse_iterator;
		using const_reverse_iterator= vector_type::const_reverse_iterator;
		using difference_type		= vector_type::difference_type;
		using Self				= Args;

	public:
		Args();
		Args(const Value& value);
		Args(const vector_type& values);
		Args(const std::initializer_list<Value>& values);
		Args(const Value& value, const Value& delim);
		Args(const Self& copy);
		~Args();

		const Value	at(size_t index) const;
		const Value	back() const;
		const Value	front() const;
		const Value	str() const;
		const Value	substr(size_t index, size_t count) const;

		const vector_type	subvec(size_t index, size_t count) const;
		const vector_type&	values() const;

		String::Stream	sstream() const;

		const size_t	count(const Value& value) const;
		const size_t	count(const_iterator first, const_iterator last, const Value& value) const;
		const size_t	indexOf(const Value& value) const;
		const size_t	size() const;

		const bool		contains(const Value& value) const;
		const bool		empty() const;
		const bool		inRange(size_t index) const;
		const bool		inRange(const_iterator it) const;

		const Self	clone() const;
		const Self	clone(size_t index, size_t count = 1) const;
		const Self	clone(const Self& other) const;
		const Self	clone(const Self& other, size_t index, size_t count = 1) const;
		const Self	clone(const_iterator first, const_iterator last) const;

	public:
		Self&	assign(const vector_type& value);
		Self&	clear();
		Self&	copy(const Self& other);
		Self&	copy(const Self& other, size_t index);
		Self&	copy(const Self& other, size_t index, size_t count);
		Self&	copy(const_iterator first, const_iterator last);
		Self&	erase(size_t index, size_t count = 1);
		Self&	erase(const_iterator it, size_t count = 1);
		Self&	erase(const_iterator first, const_iterator last);
		Self&	insert(size_t index, char value);
		Self&	insert(size_t index, const char* value);
		Self&	insert(size_t index, const Value& value);
		Self&	mergeNext(size_t index, size_t count);
		Self&	pop_back();
		Self&	pop_front();
		Self&	push_back(char value);
		Self&	push_back(const char* value);
		Self&	push_back(const Value& value);
		Self&	push_back(const vector_type& value);
		Self&	push_back(const Self& value);
		Self&	push_front(char value);
		Self&	push_front(const char* value);
		Self&	push_front(const Value& value);
		Self&	push_front(const vector_type& value);
		Self&	push_front(const Self& value);
		Self&	remove(const Value& value);
		Self&	removeAll(const Value& value);
		Self&	resize(size_t size);
		Self&	reverse();

	public:
		Args::const_iterator	find(const Value& value, size_t begin = 0) const;
		Args::const_iterator	find_first(const Value& value) const;
		Args::const_iterator	find_first_not_of(const Value& value, size_t begin = 0) const;
		Args::const_iterator	find_last(const Value& value) const;
		Args::const_iterator	find_last_not_of(const Value& value) const;

	public:
		Args::iterator					begin();
		Args::iterator					end();
		Args::const_iterator			begin() const;
		Args::const_iterator			end()	const;
		Args::const_iterator			cbegin()const;
		Args::const_iterator			cend()	const;
		Args::reverse_iterator			rbegin();
		Args::reverse_iterator			rend();
		Args::const_reverse_iterator	crbegin() const;
		Args::const_reverse_iterator	crend() const;

	public:
		inline friend std::ostream& operator<<(std::ostream& out, const Args& value)
		{
			for (const Value& s : value)
				out << s << " ";
			return out;
		}
		inline Value	operator[](size_t index) const
		{
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return Value();
		};
		inline Args &		operator=(const Args& copy)
		{
			m_values = copy.m_values;
			return (*this);
		}
		inline Args &		operator=(const vector_type& value)
		{
			(*this) = Args(value);
			return (*this);
		}

	public:
		bool equals(const Self & value) const override;
		bool lessThan(const Self & value) const override;

	private:
		vector_type m_values;
	};
}

#endif // !_ARGS_HPP_
