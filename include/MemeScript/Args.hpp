#ifndef _ARGS_HPP_
#define _ARGS_HPP_

#include <MemeCore/List.hpp>
#include <MemeScript/Export.hpp>

namespace ml
{
	class ML_SCRIPT_API Args final
		: public ITrackable
		, public IComparable<Args>
	{
	public:
		using vector_type			= List<String>;
		using iterator				= vector_type::iterator;
		using const_iterator		= vector_type::const_iterator;
		using reverse_iterator		= vector_type::reverse_iterator;
		using const_reverse_iterator= vector_type::const_reverse_iterator;
		using difference_type		= vector_type::difference_type;
		using self_type				= Args;

	public:
		Args();
		Args(const String & value);
		Args(const vector_type& values);
		Args(const std::initializer_list<String>& values);
		Args(const String & value, const String & delim);
		Args(const self_type& copy);
		~Args();

		String pop();

		const String	at(size_t index) const;
		const String	back() const;
		const String	front() const;
		const String	str() const;
		const String	substr(size_t index, size_t count) const;

		const vector_type	subvec(size_t index, size_t count) const;
		const vector_type&	values() const;

		SStream	sstream() const;

		const size_t	count(const String & value) const;
		const size_t	count(const_iterator first, const_iterator last, const String & value) const;
		const size_t	indexOf(const String & value) const;
		const size_t	size() const;

		const bool		contains(const String & value) const;
		const bool		empty() const;
		const bool		inRange(size_t index) const;
		const bool		inRange(const_iterator it) const;

		const self_type	clone() const;
		const self_type	clone(size_t index, size_t count = 1) const;
		const self_type	clone(const self_type& other) const;
		const self_type	clone(const self_type& other, size_t index, size_t count = 1) const;
		const self_type	clone(const_iterator first, const_iterator last) const;

	public:
		self_type&	assign(const vector_type& value);
		self_type&	clear();
		self_type&	copy(const self_type& other);
		self_type&	copy(const self_type& other, size_t index);
		self_type&	copy(const self_type& other, size_t index, size_t count);
		self_type&	copy(const_iterator first, const_iterator last);
		self_type&	erase(size_t index, size_t count = 1);
		self_type&	erase(const_iterator it, size_t count = 1);
		self_type&	erase(const_iterator first, const_iterator last);
		self_type&	insert(size_t index, char value);
		self_type&	insert(size_t index, CString value);
		self_type&	insert(size_t index, const String & value);
		self_type&	mergeNext(size_t index, size_t count);
		self_type&	pop_back();
		self_type&	pop_front();
		self_type&	push_back(char value);
		self_type&	push_back(CString value);
		self_type&	push_back(const String & value);
		self_type&	push_back(const vector_type& value);
		self_type&	push_back(const self_type& value);
		self_type&	push_front(char value);
		self_type&	push_front(CString value);
		self_type&	push_front(const String & value);
		self_type&	push_front(const vector_type& value);
		self_type&	push_front(const self_type& value);
		self_type&	remove(const String & value);
		self_type&	removeAll(const String & value);
		self_type&	resize(size_t size);
		self_type&	reverse();

	public:
		Args::const_iterator	find(const String & value, size_t begin = 0) const;
		Args::const_iterator	find_first(const String & value) const;
		Args::const_iterator	find_first_not_of(const String & value, size_t begin = 0) const;
		Args::const_iterator	find_last(const String & value) const;
		Args::const_iterator	find_last_not_of(const String & value) const;

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
		inline void serialize(std::ostream & out) const override
		{
			out << m_values;
		}
		inline String	operator[](size_t index) const
		{
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return String();
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
		bool equals(const self_type & value) const override;
		bool lessThan(const self_type & value) const override;

	private:
		vector_type m_values;
	};
}

#endif // !_ARGS_HPP_
