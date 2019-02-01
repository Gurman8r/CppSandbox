#ifndef _TOKEN_LIST_HPP_
#define _TOKEN_LIST_HPP_

#include <MemeScript/Token.hpp>

namespace ml
{
	class ML_SCRIPT_API TokenList final
		: public ITrackable
		, public IComparable<TokenList>
		, public IComparable<String>
	{
	public:
		using Value			= Token;
		using string_type			= String;
		using vector_type			= std::vector<Value>;
		using initializer_type		= std::initializer_list<Value>;
		using iterator				= vector_type::iterator;
		using const_iterator		= vector_type::const_iterator;
		using reverse_iterator		= vector_type::reverse_iterator;
		using const_reverse_iterator= vector_type::const_reverse_iterator;
		using difference_type		= vector_type::difference_type;
		using stream_type			= String::Stream;
		using cstring_list			= std::vector<const char*>;
		using char_list				= std::vector<char>;

	public:
		TokenList();
		TokenList(const Value& value);
		TokenList(const vector_type& values);
		TokenList(const TokenList& copy);
		~TokenList();

		const Value	at(size_t index) const;
		const Value	back() const;
		const Value	front() const;
		const String	str() const;
		const String	substr(size_t index, size_t count) const;
		const vector_type	subvec(size_t index, size_t count) const;
		const vector_type&	values() const;
		const stream_type	sstream() const;

		const size_t	count(const Value& value) const;
		const size_t	count(const_iterator first, const_iterator last, const Value& value) const;
		const size_t	indexOf(const Value& value) const;
		const size_t	size() const;

		const bool	back(const Value& value) const;
		const bool	back(const string_type& value) const;
		const bool	back(char value) const;
		const bool	front(const Value& value) const;
		const bool	front(const string_type& value) const;
		const bool	front(char value) const;

		const bool	contains(const Value& value) const;
		const bool	empty() const;
		const bool	inRange(size_t index) const;
		const bool	inRange(const_iterator it) const;
		const bool	isWrap(const Value& value) const;
		const bool	isWrap(const Value& lhs, const Value& rhs) const;

		const bool	matchChar(size_t index, char c) const;
		const bool	matchChar(const const_iterator& it, char c) const;
		const bool	matchStr(size_t index, const String& str) const;
		const bool	matchStr(const const_iterator& it, const String& str) const;
		const bool	matchPat(size_t index, const char_list& pattern) const;
		const bool	matchPat(const const_iterator& it, const char_list& pattern) const;
		const bool	matchData(size_t index, const cstring_list& data) const;
		const bool	matchData(const const_iterator& it, const cstring_list& data) const;

		TokenList	after(size_t index) const;
		TokenList	between(const Value& lhs, const Value& rhs) const;
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
		TokenList &	insert(size_t index, const Value& value);
		TokenList &	insert(iterator it, const Value& value);
		TokenList &	pop_back();
		TokenList &	pop_front();
		TokenList &	push_back(char value);
		TokenList &	push_back(const Value& value);
		TokenList &	push_back(const vector_type& value);
		TokenList &	push_back(const TokenList& value);
		TokenList &	push_front(char value);
		TokenList &	push_front(const Value& value);
		TokenList &	push_front(const vector_type& value);
		TokenList &	push_front(const TokenList& value);
		TokenList &	remove(const Value& value);
		TokenList &	removeAll(const Value& value);
		TokenList &	resize(size_t size);
		TokenList &	reverse();
		TokenList &	unwrap();
		TokenList &	unwrapIf(const Value& value);
		TokenList &	unwrapIf(const Value& lhs, const Value& rhs);
		TokenList &	wrap(const Value& value);
		TokenList &	wrap(const Value& lhs, const Value& rhs);

	public:
		const_iterator	find(const Value& value, size_t begin = 0) const;
		const_iterator	find_first(const Value& value) const;
		const_iterator	find_first_not_of(const Value& value, size_t begin = 0) const;
		const_iterator	find_last(const Value& value) const;
		const_iterator	find_last_not_of(const Value& value) const;

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
		inline const Value & operator[](size_t index) const
		{
			static Value df;
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return df;
		}
		inline Value & operator[](size_t index)
		{
			static Value df;
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
