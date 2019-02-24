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
		using iterator				= List<Token>::iterator;
		using const_iterator		= List<Token>::const_iterator;
		using reverse_iterator		= List<Token>::reverse_iterator;
		using const_reverse_iterator= List<Token>::const_reverse_iterator;
		using difference_type		= List<Token>::difference_type;

	public:
		TokenList();
		TokenList(const Token & value);
		TokenList(const List<Token>& values);
		TokenList(const TokenList& copy);
		~TokenList();

		const Token			at(size_t index) const;
		const Token			back() const;
		const Token			front() const;
		const String		str() const;
		const String		substr(size_t index, size_t count) const;
		const List<Token>	subvec(size_t index, size_t count) const;
		const List<Token> &	values() const;
		const SStream		sstream() const;

		size_t	count(const Token & value) const;
		size_t	count(const_iterator first, const_iterator last, const Token & value) const;
		size_t	indexOf(const Token & value) const;
		size_t	size() const;

		bool	back(const Token & value) const;
		bool	back(const String & value) const;
		bool	back(char value) const;

		bool	front(const Token & value) const;
		bool	front(const String & value) const;
		bool	front(char value) const;

		bool	contains(const Token & value) const;
		bool	empty() const;

		bool	inRange(size_t index) const;
		bool	inRange(const_iterator it) const;
		bool	isWrap(const Token & value) const;
		bool	isWrap(const Token & lhs, const Token & rhs) const;

		bool	matchChar(size_t index, char c) const;
		bool	matchChar(const const_iterator & it, char c) const;
		bool	matchStr(size_t index, const String & str) const;
		bool	matchStr(const const_iterator & it, const String & str) const;
		bool	matchPat(size_t index, const List<char>& pattern) const;
		bool	matchPat(const const_iterator & it, const List<char>& pattern) const;
		bool	matchData(size_t index, const List<CString>& data) const;
		bool	matchData(const const_iterator & it, const List<CString>& data) const;

		TokenList	after(size_t index) const;
		TokenList	between(const Token & lhs, const Token & rhs) const;
		TokenList	clone() const;
		TokenList	clone(size_t index, size_t count = 1) const;
		TokenList	clone(const TokenList& other) const;
		TokenList	clone(const TokenList& other, size_t index, size_t count = 1) const;
		TokenList	clone(const_iterator first, const_iterator last) const;
		TokenList	unwrapped() const;

	public:
		TokenList & assign(const List<Token>& value);
		TokenList &	clear();
		TokenList &	copy(const TokenList& other);
		TokenList &	copy(const TokenList& other, size_t index);
		TokenList &	copy(const TokenList& other, size_t index, size_t count);
		TokenList &	copy(const_iterator first, const_iterator last);
		TokenList &	erase(size_t index, size_t count = 1);
		TokenList &	erase(const_iterator it, size_t count = 1);
		TokenList &	erase(const_iterator first, const_iterator last);
		TokenList &	insert(size_t index, char value);
		TokenList &	insert(size_t index, const Token & value);
		TokenList &	insert(iterator it, const Token & value);
		TokenList &	pop_back();
		TokenList &	pop_front();
		TokenList &	push_back(char value);
		TokenList &	push_back(const Token & value);
		TokenList &	push_back(const List<Token>& value);
		TokenList &	push_back(const TokenList& value);
		TokenList &	push_front(char value);
		TokenList &	push_front(const Token & value);
		TokenList &	push_front(const List<Token>& value);
		TokenList &	push_front(const TokenList& value);
		TokenList &	remove(const Token & value);
		TokenList &	removeAll(const Token & value);
		TokenList &	resize(size_t size);
		TokenList &	reverse();
		TokenList &	unwrap();
		TokenList &	unwrapIf(const Token & value);
		TokenList &	unwrapIf(const Token & lhs, const Token & rhs);
		TokenList &	wrap(const Token & value);
		TokenList &	wrap(const Token & lhs, const Token & rhs);

	public:
		const_iterator	find(const Token & value, size_t begin = 0) const;
		const_iterator	find_first(const Token & value) const;
		const_iterator	find_first_not_of(const Token & value, size_t begin = 0) const;
		const_iterator	find_last(const Token & value) const;
		const_iterator	find_last_not_of(const Token & value) const;

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
		inline const Token & operator[](size_t index) const
		{
			static Token df;
			if (inRange(index))
			{
				return m_values.at(index);
			}
			return df;
		}
		inline Token & operator[](size_t index)
		{
			static Token df;
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
		inline TokenList & operator=(const List<Token>& value)
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
		List<Token> m_values;
	};
}

#endif // !_TOKEN_LIST_HPP_
