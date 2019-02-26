#include <MemeScript/TokenList.hpp>

namespace ml
{
	TokenList::TokenList()
		: m_values()
	{
	}
	
	TokenList::TokenList(const Token & value)
		: m_values({ value })
	{
	}
	
	TokenList::TokenList(const List<Token>& values)
		: m_values(values)
	{
	}
	
	TokenList::TokenList(const TokenList & copy)
		: m_values(copy.m_values)
	{
	}
	
	TokenList::~TokenList()
	{
	}


	const Token		TokenList::at(size_t index) const
	{
		return (*this)[index];
	}
	
	const Token		TokenList::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return Token();
	}
	
	const Token		TokenList::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return Token();
	}
	
	const String	TokenList::str() const
	{
		if (empty())
			return String();

		if (size() == 1)
			return front().data;

		String out;
		for (const_iterator it = begin(); it != end(); it++)
		{
			out += it->data + (it != end() - 1 ? " " : "");
		}
		return out;
	}

	const String	TokenList::substr(size_t index, size_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return String();
		}

		String toStr;

		for (size_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			toStr += (*it).data + (i < count - 1 ? " " : "");
		}

		return toStr;
	}
	
	const List<Token> TokenList::subvec(size_t index, size_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return List<Token>();
		}

		List<Token> out;

		for (size_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			out.push_back(*it);
		}

		return out;
	}
	
	const List<Token> &	TokenList::values() const
	{
		return m_values;
	}
	
	const SStream	TokenList::sstream() const
	{
		SStream ss;
		if (!empty())
		{
			for (auto e : (*this))
			{
				ss << e << '\n';
			}
		}
		return ss;
	}


	size_t	TokenList::count(const Token & value) const
	{
		return count(begin(), end(), value);
	}
	
	size_t	TokenList::count(const_iterator first, const_iterator last, const Token & val) const
	{
		difference_type ret = 0;

		while (first != last)
		{
			if (*first == val)
			{
				++ret;
			}

			++first;
		}

		return ret;
	}
	
	size_t	TokenList::indexOf(const Token& value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	
	size_t	TokenList::size() const
	{
		return m_values.size();
	}


	bool	TokenList::contains(const Token & value) const
	{
		return find_first(value) != end();
	}
	
	bool	TokenList::empty() const
	{
		return m_values.empty();
	}


	bool	TokenList::back(const Token & value) const
	{
		return back() == value;
	}
	
	bool	TokenList::back(const String & value) const
	{
		return back() == value;
	}
	
	bool	TokenList::back(char value) const
	{
		return back() == value;
	}


	bool	TokenList::front(const Token & value) const
	{
		return front() == value;
	}
	
	bool	TokenList::front(const String & value) const
	{
		return front() == value;
	}
	
	bool	TokenList::front(char value) const
	{
		return front() == value;
	}

	
	bool	TokenList::inRange(size_t index) const
	{
		return (index < size());
	}
	
	bool	TokenList::inRange(const_iterator it) const
	{
		return it < end();
	}
	
	bool	TokenList::isWrap(const Token & value) const
	{
		return isWrap(value, value);
	}
	
	bool	TokenList::isWrap(const Token & lhs, const Token & rhs) const
	{
		return size() >= 2 && front().type == lhs.type && back().type == rhs.type;
	}


	bool	TokenList::match_type(size_t index, char type) const
	{
		if (inRange(index))
		{
			switch (type)
			{
			case 'A':	return true;					// Any
			case 'E':	return  at(index).isOperand();	// Expression
			case 'O':	return  at(index).isOperator();	// Operator
			case 'S':	return !at(index).isOperand();	// Symbol
			default:	return  at(index).type == type;	// Match Type
			}
		}
		return false;
	}
	
	bool	TokenList::match_type(const const_iterator & it, char c) const
	{
		return match_type((it - begin()), c);
	}
	
	bool TokenList::match_type_str(const String & str) const
	{
		return match_type_str(begin(), str);
	}

	bool	TokenList::match_type_str(size_t index, const String & str) const
	{
		return match_type_str(begin() + index, str);
	}
	
	bool	TokenList::match_type_str(const const_iterator & it, const String & str) const
	{
		return match_type_list(it, List<char>(str.begin(), str.end()));
	}
	
	bool	TokenList::match_type_list(size_t index, const List<char> & pattern) const
	{
		if (!empty() && !pattern.empty())
		{
			if (index < size() && index < pattern.size())
			{
				if (pattern.size() <= size())
				{
					for (size_t i = index; i < pattern.size(); i++)
					{
						if (!match_type(i, pattern.at(i)))
						{
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	
	bool	TokenList::match_type_list(const const_iterator & it, const List<char> & pattern) const
	{
		return match_type_list((it - begin()), pattern);
	}
	
	bool TokenList::match_data(const List<CString>& data) const
	{
		return match_data(begin(), data);
	}

	bool	TokenList::match_data(size_t index, const List<CString> & data) const
	{
		if (!empty() && !data.empty())
		{
			if (index < size() && index < data.size())
			{
				if (data.size() <= size())
				{
					for (size_t i = index; i < data.size(); i++)
					{
						if (at(i).data != data.at(i))
						{
							return false;
						}
					}
					return true;
				}
			}
		}
		return false;
	}
	
	bool	TokenList::match_data(const const_iterator & it, const List<CString> & data) const
	{
		return match_data((it - begin()), data);
	}


	TokenList	TokenList::after(size_t index) const
	{
		if (inRange(index))
		{
			TokenList out;

			const_iterator it;
			for (it = begin() + index; it != end(); it++)
			{
				out.push_back(*it);
			}

			return out;
		}
		return TokenList();
	}
	
	TokenList	TokenList::between(const Token & lhs, const Token & rhs) const
	{
		const_iterator from = find_first(lhs);
		if (from != end())
		{
			const_iterator to = find_last(rhs);
			if (to != end())
			{
				TokenList out;

				for (const_iterator it = from; it != to; it++)
				{
					out.push_back(*it);
				}

				return out;
			}
		}

		return m_values;
	}
	
	TokenList	TokenList::clone() const
	{
		return TokenList().copy(*this);
	}
	
	TokenList	TokenList::clone(size_t index, size_t count) const
	{
		return TokenList().copy(*this, index, count);
	}
	
	TokenList	TokenList::clone(const TokenList & other) const
	{
		return TokenList().copy(other);
	}
	
	TokenList	TokenList::clone(const TokenList & other, size_t index, size_t count) const
	{
		return TokenList().copy(other, index, count);
	}
	
	TokenList	TokenList::clone(const_iterator first, const_iterator last) const
	{
		return TokenList().assign(List<Token>(first, last));
	}
	
	TokenList	TokenList::unwrapped() const
	{
		return TokenList(*this).unwrap();
	}

	
	TokenList &	TokenList::assign(const List<Token> & value)
	{
		m_values = value;
		return (*this);
	}
	
	TokenList &	TokenList::clear()
	{
		m_values.clear();
		return (*this);
	}
	
	TokenList &	TokenList::copy(const TokenList & other)
	{
		return copy(other, 0);
	}
	
	TokenList &	TokenList::copy(const TokenList & other, size_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
	
	TokenList &	TokenList::copy(const TokenList & other, size_t index, size_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
	
	TokenList &	TokenList::copy(const_iterator first, const_iterator last)
	{
		return assign(List<Token>(first, last));
	}
	
	TokenList &	TokenList::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
	
	TokenList &	TokenList::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
	
	TokenList &	TokenList::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
	
	TokenList &	TokenList::insert(size_t index, char value)
	{
		return insert(index, Token(value));
	}
	
	TokenList &	TokenList::insert(size_t index, const Token& value)
	{
		return insert(begin() + index, value);
	}
	
	TokenList &	TokenList::insert(iterator it, const Token & value)
	{
		m_values.insert(it, value);
		return (*this);
	}
	
	TokenList &	TokenList::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
	
	TokenList &	TokenList::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
	
	TokenList &	TokenList::push_back(char value)
	{
		return push_back(Token(value));
	}
	
	TokenList &	TokenList::push_back(const Token& value)
	{
		m_values.push_back(value);

		return (*this);
	}
	
	TokenList &	TokenList::push_back(const List<Token>& value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList &	TokenList::push_back(const TokenList & value)
	{
		return push_back(value.values());
	}
	
	TokenList &	TokenList::push_front(char value)
	{
		return push_front(Token(value));
	}
	
	TokenList &	TokenList::push_front(const Token& value)
	{
		return insert(0, value);
	}
	
	TokenList &	TokenList::push_front(const List<Token>& value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList &	TokenList::push_front(const TokenList & value)
	{
		return push_front(value.m_values);
	}
	
	TokenList &	TokenList::remove(const Token& value)
	{
		return erase(find_first(value));
	}
	
	TokenList &	TokenList::removeAll(const Token& value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
	
	TokenList &	TokenList::resize(size_t size)
	{
		m_values.resize(size);
		return (*this);
	}
	
	TokenList &	TokenList::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}
	
	TokenList &	TokenList::unwrap()
	{
		pop_front();
		return pop_back();
	}
	
	TokenList &	TokenList::unwrapIf(const Token & value)
	{
		if (isWrap(value))
		{
			return unwrap();
		}
		return (*this);
	}
	
	TokenList &	TokenList::unwrapIf(const Token & lhs, const Token & rhs)
	{
		if (isWrap(lhs, rhs))
		{
			return unwrap();
		}
		return (*this);
	}
	
	TokenList &	TokenList::wrap(const Token & value)
	{
		return wrap(value, value);
	}
	
	TokenList &	TokenList::wrap(const Token & lhs, const Token & rhs)
	{
		return push_front(lhs).push_back(rhs);
	}


	TokenList::const_iterator TokenList::find(const Token& value, size_t index) const
	{
		for (TokenList::const_iterator it = begin() + index; it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_first(const Token& value) const
	{
		for (TokenList::const_iterator it = begin(); it != end(); it++)
		{
			if ((*it) == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_first_not_of(const Token& value, size_t index) const
	{
		for (TokenList::const_iterator it = begin(); it != end(); it++)
		{
			if ((*it) != value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_last(const Token& value) const
	{
		for (TokenList::const_iterator it = end(); it != begin(); it--)
		{
			if ((*it) == value)
			{
				return it;
			}
		}
		return end();
	}
	
	TokenList::const_iterator TokenList::find_last_not_of(const Token& value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if ((*it) != value)
			{
				return it;
			}
		}
		return end();
	}


	TokenList::iterator					TokenList::begin()
	{
		return m_values.begin();
	}
	
	TokenList::iterator					TokenList::end()
	{
		return m_values.end();
	}
	
	TokenList::const_iterator			TokenList::begin() const
	{
		return m_values.begin();
	}
	
	TokenList::const_iterator			TokenList::end() const
	{
		return m_values.end();
	}
	
	TokenList::const_iterator			TokenList::cbegin() const
	{
		return m_values.cbegin();
	}
	
	TokenList::const_iterator			TokenList::cend() const
	{
		return m_values.cend();
	}
	
	TokenList::reverse_iterator			TokenList::rbegin()
	{
		return m_values.rbegin();
	}
	
	TokenList::reverse_iterator			TokenList::rend()
	{
		return m_values.rend();
	}
	
	TokenList::const_reverse_iterator	TokenList::crbegin() const
	{
		return m_values.crbegin();
	}
	
	TokenList::const_reverse_iterator	TokenList::crend() const
	{
		return m_values.crend();
	}


	void TokenList::serialize(std::ostream & out) const
	{
		TokenList::const_iterator it;
		for (it = begin(); it != end(); it++)
		{
			if ((*it) == '\n')
			{
				out << '\n';
				continue;
			}
			out << (*it) << (it != end() - 1 ? ", " : "");
		}
	}

	bool TokenList::equals(const TokenList & value) const
	{
		if (size() == value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool TokenList::lessThan(const TokenList & value) const
	{
		if (size() != value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}
		return size() < value.size();
	}

	bool TokenList::equals(const String & value) const
	{
		return match_type_str(begin(), value);
	}

	bool TokenList::lessThan(const String & value) const
	{
		if (!equals(value))
		{
			return str() < value;
		}
		return false;
	}
}