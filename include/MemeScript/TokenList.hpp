#ifndef _TOKEN_LIST_HPP_
#define _TOKEN_LIST_HPP_

#include <MemeScript/Token.hpp>
#include <MemeCore/List.hpp>

namespace ml
{
	class ML_SCRIPT_API TokenList final
		: public ITrackable
		, public IComparable<TokenList>
		, public IComparable<String>
	{
	public:
		using value_type			= Token;
		using vector_type			= List<value_type>;
		using cstring_list			= List<CString>;
		using char_list				= List<char>;
		using initializer_type		= std::initializer_list<value_type>;
		using iterator				= vector_type::iterator;
		using const_iterator		= vector_type::const_iterator;
		using reverse_iterator		= vector_type::reverse_iterator;
		using const_reverse_iterator= vector_type::const_reverse_iterator;
		using difference_type		= vector_type::difference_type;
		using stream_type			= SStream;

	public:
		TokenList();
		TokenList(const value_type& value);
		TokenList(const vector_type& values);
		TokenList(const TokenList& copy);
		~TokenList();

		const value_type	at(size_t index) const;
		const value_type	back() const;
		const value_type	front() const;
		const String		str() const;
		const String		substr(size_t index, size_t count) const;
		const vector_type	subvec(size_t index, size_t count) const;
		const vector_type&	values() const;
		const stream_type	sstream() const;

		const size_t	count(const value_type& value) const;
		const size_t	count(const_iterator first, const_iterator last, const value_type& value) const;
		const size_t	indexOf(const value_type& value) const;
		const size_t	size() const;

		const bool	back(const value_type& value) const;
		const bool	back(const String& value) const;
		const bool	back(char value) const;
		const bool	front(const value_type& value) const;
		const bool	front(const String& value) const;
		const bool	front(char value) const;

		const bool	contains(const value_type& value) const;
		const bool	empty() const;
		const bool	inRange(size_t index) const;
		const bool	inRange(const_iterator it) const;
		const bool	isWrap(const value_type& value) const;
		const bool	isWrap(const value_type& lhs, const value_type& rhs) const;

		const bool	matchChar(size_t index, char c) const;
		const bool	matchChar(const const_iterator& it, char c) const;
		const bool	matchStr(size_t index, const String& str) const;
		const bool	matchStr(const const_iterator& it, const String& str) const;
		const bool	matchPat(size_t index, const char_list& pattern) const;
		const bool	matchPat(const const_iterator& it, const char_list& pattern) const;
		const bool	matchData(size_t index, const cstring_list& data) const;
		const bool	matchData(const const_iterator& it, const cstring_list& data) const;

		TokenList	after(size_t index) const;
		TokenList	between(const value_type& lhs, const value_type& rhs) const;
		TokenList	clone() const;
		TokenList	clone(size_t index, size_t count = 1) const;
		TokenList	clone(const TokenList& other) const;
		TokenList	clone(const TokenList& other, size_t index, size_t count = 1) const;
		TokenList	clone(const_iterator first, const_iterator last) const;
		TokenList	unwrapped() const;

	public:
		TokenList & assign(const vector_type& value);
		TokenList &	clear();
		TokenList &	copy(const TokenList& other);
		TokenList &	copy(const TokenList& other, size_t index);
		TokenList &	copy(const TokenList& other, size_t index, size_t count);
		TokenList &	copy(const_iterator first, const_iterator last);
		TokenList &	erase(size_t index, size_t count = 1);
		TokenList &	erase(const_iterator it, size_t count = 1);
		TokenList &	erase(const_iterator first, const_iterator last);
		TokenList &	insert(size_t index, char value);
		TokenList &	insert(size_t index, const value_type& value);
		TokenList &	insert(iterator it, const value_type& value);
		TokenList &	pop_back();
		TokenList &	pop_front();
		TokenList &	push_back(char value);
		TokenList &	push_back(const value_type& value);
		TokenList &	push_back(const vector_type& value);
		TokenList &	push_back(const TokenList& value);
		TokenList &	push_front(char value);
		TokenList &	push_front(const value_type& value);
		TokenList &	push_front(const vector_type& value);
		TokenList &	push_front(const TokenList& value);
		TokenList &	remove(const value_type& value);
		TokenList &	removeAll(const value_type& value);
		TokenList &	resize(size_t size);
		TokenList &	reverse();
		TokenList &	unwrap();
		TokenList &	unwrapIf(const value_type& value);
		TokenList &	unwrapIf(const value_type& lhs, const value_type& rhs);
		TokenList &	wrap(const value_type& value);
		TokenList &	wrap(const value_type& lhs, const value_type& rhs);

	public:
		const_iterator	find(const value_type& value, size_t begin = 0) const;
		const_iterator	find_first(const value_type& value) const;
		const_iterator	find_first_not_of(const value_type& value, size_t begin = 0) const;
		const_iterator	find_last(const value_type& value) const;
		const_iterator	find_last_not_of(const value_type& value) const;

	public:
		iterator				begin();
		iterator				end();
		const_iterator			begin() const;
		const_iterator			end()	const;
		const_iterator			cbegin()const;
		const_iterator			cend()	const;
		reverse_iterator		rbegin();
		reverse_iterator		rend();
		const_reverse_iterator	crbegin() const;
		const_reverse_iterator	crend() const;

	public:
		inline const value_type & operator[](size_t index) const
		{
			static value_type df;
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return df;
		}
		inline value_type & operator[](size_t index)
		{
			static value_type df;
			if (inRange(index))
			{
				return m_values[index];
			}
			return df;
		}
		inline TokenList & operator=(const TokenList& copy)
		{
			return this->copy(copy);
		}
		inline TokenList & operator=(const vector_type& value)
		{
			return this->assign(value);
		}

	public:
		void serialize(std::ostream & out) const override;

		bool equals(const TokenList & value) const override;
		bool lessThan(const TokenList & value) const override;

		bool equals(const String & value) const override;
		bool lessThan(const String & value) const override;

	private:
		vector_type m_values;
	};
}

#endif // !_TOKEN_LIST_HPP_
