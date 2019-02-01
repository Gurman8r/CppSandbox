#include <MemeScript/TokenList.hpp>

namespace ml
{
	TokenList::TokenList()
		: m_values()
	{
	}
	
	TokenList::TokenList(const Value & value)
		: m_values({ value })
	{
	}
	
	TokenList::TokenList(const vector_type& values)
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


	const TokenList::Value		TokenList::at(size_t index) const
	{
		return (*this)[index];
	}
	
	const TokenList::Value		TokenList::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return Value();
	}
	
	const TokenList::Value		TokenList::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return Value();
	}
	
	const String				TokenList::str() const
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
	
	const String				TokenList::substr(size_t index, size_t count) const
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
	
	const TokenList::vector_type	TokenList::subvec(size_t index, size_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return vector_type();
		}

		vector_type out;

		for (size_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			out.push_back(*it);
		}

		return out;
	}
	
	const TokenList::vector_type&	TokenList::values() const
	{
		return m_values;
	}
	
	const TokenList::stream_type	TokenList::sstream() const
	{
		String::Stream stream;

		if (!empty())
		{
			for (auto e : (*this))
			{
				stream << e << '\n';
			}
		}

		return stream;
	}


	const size_t	TokenList::count(const Value & value) const
	{
		return count(begin(), end(), value);
	}
	
	const size_t	TokenList::count(const_iterator first, const_iterator last, const Value & val) const
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
	
	const size_t	TokenList::indexOf(const Value& value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	
	const size_t	TokenList::size() const
	{
		return m_values.size();
	}


	const bool	TokenList::contains(const Value & value) const
	{
		return find_first(value) != end();
	}
	
	const bool	TokenList::empty() const
	{
		return m_values.empty();
	}


	const bool	TokenList::back(const Value & value) const
	{
		return back() == value;
	}
	
	const bool	TokenList::back(const string_type & value) const
	{
		return back() == value;
	}
	
	const bool	TokenList::back(char value) const
	{
		return back() == value;
	}


	const bool	TokenList::front(const Value & value) const
	{
		return front() == value;
	}
	
	const bool	TokenList::front(const string_type & value) const
	{
		return front() == value;
	}
	
	const bool	TokenList::front(char value) const
	{
		return front() == value;
	}

	
	const bool	TokenList::inRange(size_t index) const
	{
		return (index < size());
	}
	
	const bool	TokenList::inRange(const_iterator it) const
	{
		return it < end();
	}
	
	const bool	TokenList::isWrap(const Value & value) const
	{
		return isWrap(value, value);
	}
	
	const bool	TokenList::isWrap(const Value & lhs, const Value & rhs) const
	{
		return size() >= 2 && front().type == lhs.type && back().type == rhs.type;
	}


	const bool	TokenList::matchChar(size_t index, char c) const
	{
		if (inRange(index))
		{
			switch (c)
			{
			case 'A':	// Any
				return true;

			case 'E':	// Operand
				return at(index).isOperand();

			case 'O':	// Operator
				return at(index).isOperator();

			case 'S':	// Non-Operand (Symbol?)
				return !at(index).isOperand();

			default:	// TokenType
				return at(index).type == c;
			}
		}

		return false;
	}
	
	const bool	TokenList::matchChar(const const_iterator & it, char c) const
	{
		return matchChar((it - begin()), c);
	}
	
	const bool	TokenList::matchStr(size_t index, const String & str) const
	{
		return matchStr(begin() + index, str);
	}
	
	const bool	TokenList::matchStr(const const_iterator & it, const String & str) const
	{
		return matchPat(it, char_list(str.begin(), str.end()));
	}
	
	const bool	TokenList::matchPat(size_t index, const char_list & pattern) const
	{
		if (!empty() && !pattern.empty())
		{
			if (index < size() && index < pattern.size())
			{
				if (pattern.size() <= size())
				{
					for (size_t i = index; i < pattern.size(); i++)
					{
						if (!matchChar(i, pattern.at(i)))
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
	
	const bool	TokenList::matchPat(const const_iterator & it, const char_list & pattern) const
	{
		return matchPat((it - begin()), pattern);
	}
	
	const bool	TokenList::matchData(size_t index, const cstring_list & data) const
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
	
	const bool	TokenList::matchData(const const_iterator & it, const cstring_list & data) const
	{
		return matchData((it - begin()), data);
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
	
	TokenList	TokenList::between(const Value & lhs, const Value & rhs) const
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
		return TokenList().assign(vector_type(first, last));
	}
	
	TokenList	TokenList::unwrapped() const
	{
		return TokenList(*this).unwrap();
	}

	
	TokenList&	TokenList::assign(const vector_type & value)
	{
		m_values = value;
		return (*this);
	}
	
	TokenList&	TokenList::clear()
	{
		m_values.clear();
		return (*this);
	}
	
	TokenList&	TokenList::copy(const TokenList& other)
	{
		return copy(other, 0);
	}
	
	TokenList&	TokenList::copy(const TokenList& other, size_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
	
	TokenList&	TokenList::copy(const TokenList& other, size_t index, size_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
	
	TokenList&	TokenList::copy(const_iterator first, const_iterator last)
	{
		return assign(vector_type(first, last));
	}
	
	TokenList&	TokenList::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
	
	TokenList&	TokenList::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
	
	TokenList&	TokenList::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
	
	TokenList&	TokenList::insert(size_t index, char value)
	{
		return insert(index, Value(value));
	}
	
	TokenList&	TokenList::insert(size_t index, const Value& value)
	{
		return insert(begin() + index, value);
	}
	
	TokenList&	TokenList::insert(iterator it, const Value & value)
	{
		m_values.insert(it, value);
		return (*this);
	}
	
	TokenList&	TokenList::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
	
	TokenList&	TokenList::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
	
	TokenList&	TokenList::push_back(char value)
	{
		return push_back(Value(value));
	}
	
	TokenList&	TokenList::push_back(const Value& value)
	{
		m_values.push_back(value);

		return (*this);
	}
	
	TokenList&	TokenList::push_back(const vector_type& value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList&	TokenList::push_back(const TokenList& value)
	{
		return push_back(value.values());
	}
	
	TokenList&	TokenList::push_front(char value)
	{
		return push_front(Value(value));
	}
	
	TokenList&	TokenList::push_front(const Value& value)
	{
		return insert(0, value);
	}
	
	TokenList&	TokenList::push_front(const vector_type& value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	
	TokenList&	TokenList::push_front(const TokenList& value)
	{
		return push_front(value.m_values);
	}
	
	TokenList&	TokenList::remove(const Value& value)
	{
		return erase(find_first(value));
	}
	
	TokenList&	TokenList::removeAll(const Value& value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
	
	TokenList&	TokenList::resize(size_t size)
	{
		m_values.resize(size);
		return (*this);
	}
	
	TokenList&	TokenList::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}
	
	TokenList&	TokenList::unwrap()
	{
		pop_front();
		return pop_back();
	}
	
	TokenList&	TokenList::unwrapIf(const Value & value)
	{
		if (isWrap(value))
		{
			return unwrap();
		}
		return (*this);
	}
	
	TokenList&	TokenList::unwrapIf(const Value & lhs, const Value & rhs)
	{
		if (isWrap(lhs, rhs))
		{
			return unwrap();
		}
		return (*this);
	}
	
	TokenList&	TokenList::wrap(const Value & value)
	{
		return wrap(value, value);
	}
	
	TokenList&	TokenList::wrap(const Value & lhs, const Value & rhs)
	{
		return push_front(lhs).push_back(rhs);
	}


	TokenList::const_iterator TokenList::find(const Value& value, size_t index) const
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
	
	TokenList::const_iterator TokenList::find_first(const Value& value) const
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
	
	TokenList::const_iterator TokenList::find_first_not_of(const Value& value, size_t index) const
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
	
	TokenList::const_iterator TokenList::find_last(const Value& value) const
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
	
	TokenList::const_iterator TokenList::find_last_not_of(const Value& value) const
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
		return matchStr(begin(), value);
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