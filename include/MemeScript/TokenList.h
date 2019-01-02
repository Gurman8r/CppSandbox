#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#include <MemeScript/Token.h>
#include <sstream>
#include <initializer_list>

namespace ml
{
	class ML_SCRIPT_API TokenList final
		: public ITrackable
		, public IComparable<TokenList>
	{
	public:
		using value_type			= Token;
		using string_type			= std::string;
		using vector_type			= std::vector<value_type>;
		using initializer_type		= std::initializer_list<value_type>;
		using iterator				= vector_type::iterator;
		using const_iterator		= vector_type::const_iterator;
		using reverse_iterator		= vector_type::reverse_iterator;
		using const_reverse_iterator= vector_type::const_reverse_iterator;
		using difference_type		= vector_type::difference_type;
		using self_type				= TokenList;
		using stream_type			= std::stringstream;
		using cstring_list			= std::vector<const char*>;
		using char_list				= std::vector<char>;

	public:
		TokenList();
		TokenList(const value_type& value);
		TokenList(const vector_type& values);
		TokenList(const self_type& copy);
		~TokenList();

		const value_type	at(uint32_t index) const;
		const value_type	back() const;
		const value_type	front() const;
		const std::string	str() const;
		const std::string	substr(uint32_t index, uint32_t count) const;
		const vector_type	subvec(uint32_t index, uint32_t count) const;
		const vector_type&	values() const;
		const stream_type	sstream() const;

		const uint32_t	count(const value_type& value) const;
		const uint32_t	count(const_iterator first, const_iterator last, const value_type& value) const;
		const uint32_t	indexOf(const value_type& value) const;
		const uint32_t	size() const;

		const bool	back(const value_type& value) const;
		const bool	back(const string_type& value) const;
		const bool	back(char value) const;
		const bool	front(const value_type& value) const;
		const bool	front(const string_type& value) const;
		const bool	front(char value) const;

		const bool	contains(const value_type& value) const;
		const bool	empty() const;
		const bool	inRange(uint32_t index) const;
		const bool	inRange(const_iterator it) const;
		const bool	isWrap(const value_type& value) const;
		const bool	isWrap(const value_type& lhs, const value_type& rhs) const;

		const bool	matchC(uint32_t index, char c) const;
		const bool	matchC(const const_iterator& it, char c) const;
		const bool	matchS(uint32_t index, const std::string& str) const;
		const bool	matchS(const const_iterator& it, const std::string& str) const;
		const bool	matchP(uint32_t index, const char_list& pattern) const;
		const bool	matchP(const const_iterator& it, const char_list& pattern) const;
		const bool	matchD(uint32_t index, const cstring_list& data) const;
		const bool	matchD(const const_iterator& it, const cstring_list& data) const;

		self_type	after(uint32_t index) const;
		self_type	between(const value_type& lhs, const value_type& rhs) const;
		self_type	clone() const;
		self_type	clone(uint32_t index, uint32_t count = 1) const;
		self_type	clone(const self_type& other) const;
		self_type	clone(const self_type& other, uint32_t index, uint32_t count = 1) const;
		self_type	clone(const_iterator first, const_iterator last) const;
		self_type	unwrapped() const;

	public:
		self_type & assign(const vector_type& value);
		self_type&	clear();
		self_type&	copy(const self_type& other);
		self_type&	copy(const self_type& other, uint32_t index);
		self_type&	copy(const self_type& other, uint32_t index, uint32_t count);
		self_type&	copy(const_iterator first, const_iterator last);
		self_type&	erase(uint32_t index, uint32_t count = 1);
		self_type&	erase(const_iterator it, uint32_t count = 1);
		self_type&	erase(const_iterator first, const_iterator last);
		self_type&	insert(uint32_t index, char value);
		self_type&	insert(uint32_t index, const value_type& value);
		self_type&	insert(iterator it, const value_type& value);
		self_type&	pop_back();
		self_type&	pop_front();
		self_type&	push_back(char value);
		self_type&	push_back(const value_type& value);
		self_type&	push_back(const vector_type& value);
		self_type&	push_back(const self_type& value);
		self_type&	push_front(char value);
		self_type&	push_front(const value_type& value);
		self_type&	push_front(const vector_type& value);
		self_type&	push_front(const self_type& value);
		self_type&	remove(const value_type& value);
		self_type&	removeAll(const value_type& value);
		self_type&	resize(uint32_t size);
		self_type&	reverse();
		self_type&	unwrap();
		self_type&	unwrapIf(const value_type& value);
		self_type&	unwrapIf(const value_type& lhs, const value_type& rhs);
		self_type&	wrap(const value_type& value);
		self_type&	wrap(const value_type& lhs, const value_type& rhs);

	public:
		const_iterator	find(const value_type& value, uint32_t begin = 0) const;
		const_iterator	find_first(const value_type& value) const;
		const_iterator	find_first_not_of(const value_type& value, uint32_t begin = 0) const;
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
		friend std::ostream& operator<<(std::ostream& out, const TokenList& value);
		inline const value_type&operator[](uint32_t index) const
		{
			static value_type df;

			if (inRange(index))
			{
				return m_values.at(index);
			}
			return df;
		};
		inline value_type&		operator[](uint32_t index)
		{
			static value_type df;

			if (inRange(index))
			{
				return m_values[index];
			}
			return df;
		};
		inline TokenList&			operator=(const TokenList& copy)
		{
			return this->copy(copy);
		}
		inline TokenList&			operator=(const vector_type& value)
		{
			return this->assign(value);
		}

	public:
		bool equals(const self_type & value) const override;
		bool lessThan(const self_type & value) const override;

		inline friend bool operator==(const TokenList& lhs, const std::string& rhs);
		inline friend bool operator!=(const TokenList& lhs, const std::string& rhs);

	private:
		vector_type m_values;
	};
}

#endif // !_TOKEN_LIST_H_
