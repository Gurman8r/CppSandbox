#include <MemeScript/TokenList.h>

namespace ml
{
	TokenList::TokenList()
		: m_values()
	{
	}
	TokenList::TokenList(const value_type & value)
		: m_values({ value })
	{
	}
	TokenList::TokenList(const vector_type& values)
		: m_values(values)
	{
	}
	TokenList::TokenList(const self_type & copy)
		: m_values(copy.m_values)
	{
	}
	TokenList::~TokenList()
	{
	}

	const TokenList::value_type		TokenList::at(uint32_t index) const
	{
		return (*this)[index];
	}
	const TokenList::value_type		TokenList::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return value_type();
	}
	const TokenList::value_type		TokenList::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return value_type();
	}
	const std::string				TokenList::str() const
	{
		if (empty())
			return std::string();

		if (size() == 1)
			return front().data;

		std::string out;

		const_iterator it;
		for (it = begin(); it != end(); it++)
		{
			out += it->data + (it != end() - 1 ? " " : "");
		}

		return out;
	}
	const std::string				TokenList::substr(uint32_t index, uint32_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return std::string();
		}

		std::string toStr;

		for (uint32_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			toStr += (*it).data + (i < count - 1 ? " " : "");
		}

		return toStr;
	}
	const TokenList::vector_type	TokenList::subvec(uint32_t index, uint32_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return vector_type();
		}

		vector_type out;

		for (uint32_t i = 0; i < count; i++)
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
		std::stringstream stream;

		if (!empty())
		{
			for (auto e : (*this))
			{
				stream << e << '\n';
			}
		}

		return stream;
	}

	const uint32_t	TokenList::count(const value_type & value) const
	{
		return count(begin(), end(), value);
	}
	const uint32_t	TokenList::count(const_iterator first, const_iterator last, const value_type & val) const
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
	const uint32_t	TokenList::indexOf(const value_type& value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	const uint32_t	TokenList::size() const
	{
		return m_values.size();
	}

	const bool	TokenList::contains(const value_type & value) const
	{
		return find_first(value) != end();
	}
	const bool	TokenList::empty() const
	{
		return m_values.empty();
	}


	const bool	TokenList::back(const value_type & value) const
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

	const bool	TokenList::front(const value_type & value) const
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

	const bool	TokenList::inRange(uint32_t index) const
	{
		return (index < size());
	}
	const bool	TokenList::inRange(const_iterator it) const
	{
		return it < end();
	}
	const bool	TokenList::isWrap(const value_type & value) const
	{
		return isWrap(value, value);
	}
	const bool	TokenList::isWrap(const value_type & lhs, const value_type & rhs) const
	{
		return size() >= 2 && front().type == lhs.type && back().type == rhs.type;
	}

	const bool	TokenList::matchC(uint32_t index, char c) const
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

			default:	// Type
				return at(index).type == c;
			}
		}

		return false;
	}
	const bool	TokenList::matchC(const const_iterator & it, char c) const
	{
		return matchC((it - begin()), c);
	}
	const bool	TokenList::matchS(uint32_t index, const std::string & str) const
	{
		return matchS(begin() + index, str);
	}
	const bool	TokenList::matchS(const const_iterator & it, const std::string & str) const
	{
		return matchP(it, char_list(str.begin(), str.end()));
	}
	const bool	TokenList::matchP(uint32_t index, const char_list & pattern) const
	{
		if (!empty() && !pattern.empty())
		{
			if (index < size() && index < pattern.size())
			{
				if (pattern.size() <= size())
				{
					for (uint32_t i = index; i < pattern.size(); i++)
					{
						if (!matchC(i, pattern.at(i)))
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
	const bool	TokenList::matchP(const const_iterator & it, const char_list & pattern) const
	{
		return matchP((it - begin()), pattern);
	}
	const bool	TokenList::matchD(uint32_t index, const cstring_list & data) const
	{
		if (!empty() && !data.empty())
		{
			if (index < size() && index < data.size())
			{
				if (data.size() <= size())
				{
					for (uint32_t i = index; i < data.size(); i++)
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
	const bool	TokenList::matchD(const const_iterator & it, const cstring_list & data) const
	{
		return matchD((it - begin()), data);
	}

	TokenList::self_type	TokenList::after(uint32_t index) const
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
	TokenList::self_type	TokenList::between(const value_type & lhs, const value_type & rhs) const
	{
		const_iterator from = find_first(lhs);
		if (from != end())
		{
			const_iterator to = find_last(rhs);
			if (to != end())
			{
				self_type out;

				for (const_iterator it = from; it != to; it++)
				{
					out.push_back(*it);
				}

				return out;
			}
		}

		return m_values;
	}
	TokenList::self_type	TokenList::clone() const
	{
		return TokenList().copy(*this);
	}
	TokenList::self_type	TokenList::clone(uint32_t index, uint32_t count) const
	{
		return TokenList().copy(*this, index, count);
	}
	TokenList::self_type	TokenList::clone(const self_type & other) const
	{
		return TokenList().copy(other);
	}
	TokenList::self_type	TokenList::clone(const self_type & other, uint32_t index, uint32_t count) const
	{
		return TokenList().copy(other, index, count);
	}
	TokenList::self_type	TokenList::clone(const_iterator first, const_iterator last) const
	{
		return TokenList().assign(vector_type(first, last));
	}
	TokenList::self_type	TokenList::unwrapped() const
	{
		return self_type(*this).unwrap();
	}

	TokenList::self_type&	TokenList::assign(const vector_type & value)
	{
		m_values = value;
		return (*this);
	}
	TokenList::self_type&	TokenList::clear()
	{
		m_values.clear();
		return (*this);
	}
	TokenList::self_type&	TokenList::copy(const self_type& other)
	{
		return copy(other, 0);
	}
	TokenList::self_type&	TokenList::copy(const self_type& other, uint32_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
	TokenList::self_type&	TokenList::copy(const self_type& other, uint32_t index, uint32_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
	TokenList::self_type&	TokenList::copy(const_iterator first, const_iterator last)
	{
		return assign(vector_type(first, last));
	}
	TokenList::self_type&	TokenList::erase(uint32_t index, uint32_t count)
	{
		return erase(begin() + index, count);
	}
	TokenList::self_type&	TokenList::erase(const_iterator it, uint32_t count)
	{
		return erase(it, it + count);
	}
	TokenList::self_type&	TokenList::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::insert(uint32_t index, char value)
	{
		return insert(index, value_type(value));
	}
	TokenList::self_type&	TokenList::insert(uint32_t index, const value_type& value)
	{
		return insert(begin() + index, value);
	}
	TokenList::self_type&	TokenList::insert(iterator it, const value_type & value)
	{
		m_values.insert(it, value);
		return (*this);
	}
	TokenList::self_type&	TokenList::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
	TokenList::self_type&	TokenList::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
	TokenList::self_type&	TokenList::push_back(char value)
	{
		return push_back(value_type(value));
	}
	TokenList::self_type&	TokenList::push_back(const value_type& value)
	{
		m_values.push_back(value);

		return (*this);
	}
	TokenList::self_type&	TokenList::push_back(const vector_type& value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::push_back(const self_type& value)
	{
		return push_back(value.values());
	}
	TokenList::self_type&	TokenList::push_front(char value)
	{
		return push_front(value_type(value));
	}
	TokenList::self_type&	TokenList::push_front(const value_type& value)
	{
		return insert(0, value);
	}
	TokenList::self_type&	TokenList::push_front(const vector_type& value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::push_front(const self_type& value)
	{
		return push_front(value.m_values);
	}
	TokenList::self_type&	TokenList::remove(const value_type& value)
	{
		return erase(find_first(value));
	}
	TokenList::self_type&	TokenList::removeAll(const value_type& value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::resize(uint32_t size)
	{
		m_values.resize(size);
		return (*this);
	}
	TokenList::self_type&	TokenList::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}
	TokenList::self_type&	TokenList::unwrap()
	{
		pop_front();
		return pop_back();
	}
	TokenList::self_type&	TokenList::unwrapIf(const value_type & value)
	{
		if (isWrap(value))
		{
			return unwrap();
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::unwrapIf(const value_type & lhs, const value_type & rhs)
	{
		if (isWrap(lhs, rhs))
		{
			return unwrap();
		}
		return (*this);
	}
	TokenList::self_type&	TokenList::wrap(const value_type & value)
	{
		return wrap(value, value);
	}
	TokenList::self_type&	TokenList::wrap(const value_type & lhs, const value_type & rhs)
	{
		return push_front(lhs).push_back(rhs);
	}


	TokenList::const_iterator TokenList::find(const value_type& value, uint32_t index) const
	{
		for (auto it = begin() + index; it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	TokenList::const_iterator TokenList::find_first(const value_type& value) const
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	TokenList::const_iterator TokenList::find_first_not_of(const value_type& value, uint32_t index) const
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (*it != value)
			{
				return it;
			}
		}
		return end();
	}
	TokenList::const_iterator TokenList::find_last(const value_type& value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	TokenList::const_iterator TokenList::find_last_not_of(const value_type& value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if (*it != value)
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


	std::ostream & operator<<(std::ostream & out, const TokenList & value)
	{
		TokenList::const_iterator it;
		for (it = value.begin(); it != value.end(); it++)
		{
			if ((*it) == '\n')
			{
				out << '\n';
				continue;
			}

			out << (*it) << (it != value.end() - 1 ? ", " : "");
		}

		return out;
	}

	bool TokenList::equals(const self_type & value) const
	{
		if (size() == value.size())
		{
			for (std::size_t i = 0, imax = size(); i < imax; i++)
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

	bool TokenList::lessThan(const self_type & value) const
	{
		if (size() != value.size())
		{
			for (std::size_t i = 0, imax = size(); i < imax; i++)
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

	bool operator==(const TokenList & lhs, const std::string & rhs)
	{
		return lhs.matchS(lhs.begin(), rhs);
	}

	bool operator!=(const TokenList & lhs, const std::string & rhs)
	{
		return !(lhs == rhs);
	}
}