#include <MemeScript/Args.h>
#include <MemeScript/StringUtility.h>

namespace ml
{
	Args::Args()
		: m_values()
	{
	}
	Args::Args(const value_type & value)
		: m_values({ value })
	{
	}
	Args::Args(const vector_type& values)
		: m_values(values)
	{
	}
	Args::Args(const std::initializer_list<value_type>& values)
		: m_values(values)
	{
	}
	Args::Args(const value_type & value, const value_type & delim)
		: m_values(StringUtility::Split(value, delim))
	{
	}
	Args::Args(const self_type & copy)
		: m_values(copy.m_values)
	{
	}
	Args::~Args()
	{
	}

	const Args::value_type	Args::at(size_t index) const
	{
		return (*this)[index];
	}
	const Args::value_type	Args::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return value_type();
	}
	const Args::value_type	Args::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return value_type();
	}
	const Args::value_type	Args::str() const
	{
		value_type out;
		for (const value_type& it : (*this))
		{
			out += it + " ";
		}
		return out;
	}
	const Args::value_type	Args::substr(size_t index, size_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return value_type();
		}

		value_type toStr;

		for (size_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			toStr += (*it) + (i < count - 1 ? " " : "");
		}

		return toStr;
	}

	const Args::vector_type Args::subvec(size_t index, size_t count) const
	{
		auto from = (begin() + index);

		if (!inRange(from) || !inRange(from + count))
		{
			return vector_type();
		}

		vector_type toStr;

		for (size_t i = 0; i < count; i++)
		{
			auto it = (from + i);

			if (it == end())
				break;

			toStr.push_back((*it) + (i < count - 1 ? " " : ""));
		}

		return toStr;
	}
	const Args::vector_type& Args::values() const
	{
		return m_values;
	}

	std::stringstream Args::sstream() const
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

	const size_t	Args::count(const value_type & value) const
	{
		return count(begin(), end(), value);
	}
	const size_t	Args::count(const_iterator first, const_iterator last, const value_type & val) const
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
	const size_t	Args::indexOf(const value_type& value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	const size_t	Args::size() const
	{
		return m_values.size();
	}

	const bool		Args::contains(const value_type & value) const
	{
		return find_first(value) != end();
	}
	const bool		Args::empty() const
	{
		return m_values.empty();
	}
	const bool		Args::inRange(size_t index) const
	{
		return (index < size());
	}
	const bool		Args::inRange(const_iterator it) const
	{
		return it < end();
	}

	const Args::self_type Args::clone() const
	{
		return Args().copy(*this);
	}
	const Args::self_type Args::clone(size_t index, size_t count) const
	{
		return Args().copy(*this, index, count);
	}
	const Args::self_type Args::clone(const self_type & other) const
	{
		return Args().copy(other);
	}
	const Args::self_type Args::clone(const self_type & other, size_t index, size_t count) const
	{
		return Args().copy(other, index, count);
	}
	const Args::self_type Args::clone(const_iterator first, const_iterator last) const
	{
		return Args().assign(vector_type(first, last));
	}


	Args::self_type&	Args::assign(const vector_type & value)
	{
		m_values = value;

		return (*this);
	}
	Args::self_type&	Args::clear()
	{
		m_values.clear();
		return (*this);
	}
	Args::self_type&	Args::copy(const self_type& other)
	{
		return copy(other, 0);
	}
	Args::self_type&	Args::copy(const self_type& other, size_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
	Args::self_type&	Args::copy(const self_type& other, size_t index, size_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
	Args::self_type&	Args::copy(const_iterator first, const_iterator last)
	{
		return assign(vector_type(first, last));
	}
	Args::self_type&	Args::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
	Args::self_type&	Args::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
	Args::self_type&	Args::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
	Args::self_type&	Args::insert(size_t index, char value)
	{
		return insert(index, value_type(1, value));
	}
	Args::self_type&	Args::insert(size_t index, const char * value)
	{
		return insert(index, value_type(value));
	}
	Args::self_type&	Args::insert(size_t index, const value_type& value)
	{
		m_values.insert(begin() + index, value);
		return (*this);
	}
	Args::self_type&	Args::mergeNext(size_t index, size_t count)
	{
		auto from = (begin() + index);
		auto to = (from + count);

		if (!inRange(from) || !inRange(to))
			return (*this);

		Args tmp;

		for (auto it = begin(); it != from; it++)
		{
			tmp.push_back(*it);
		}

		tmp.push_back(substr(index, count));

		for (auto it = to; it != end(); it++)
		{
			tmp.push_back(*it);
		}

		(*this) = Args(tmp);

		return (*this);
	}
	Args::self_type&	Args::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
	Args::self_type&	Args::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
	Args::self_type&	Args::push_back(char value)
	{
		return push_back(value_type(1, value));
	}
	Args::self_type&	Args::push_back(const char* value)
	{
		return push_back(value_type(value));
	}
	Args::self_type&	Args::push_back(const value_type& value)
	{
		m_values.push_back(value);
		return (*this);
	}
	Args::self_type&	Args::push_back(const vector_type& value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	Args::self_type&	Args::push_back(const self_type& value)
	{
		return push_back(value.m_values);
	}
	Args::self_type&	Args::push_front(char value)
	{
		return push_front(value_type(1, value));
	}
	Args::self_type&	Args::push_front(const char* value)
	{
		return push_front(value_type(value));
	}
	Args::self_type&	Args::push_front(const value_type& value)
	{
		return insert(0, value);
	}
	Args::self_type&	Args::push_front(const vector_type& value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
	Args::self_type&	Args::push_front(const self_type& value)
	{
		return push_front(value.m_values);
	}
	Args::self_type&	Args::remove(const value_type& value)
	{
		return erase(find_first(value));
	}
	Args::self_type&	Args::removeAll(const value_type& value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
	Args::self_type&	Args::resize(size_t size)
	{
		m_values.resize(size);
		return (*this);
	}
	Args::self_type&	Args::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}


	Args::const_iterator Args::find(const value_type& value, size_t index) const
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
	Args::const_iterator Args::find_first(const value_type& value) const
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
	Args::const_iterator Args::find_first_not_of(const value_type& value, size_t index) const
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
	Args::const_iterator Args::find_last(const value_type& value) const
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
	Args::const_iterator Args::find_last_not_of(const value_type& value) const
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


	Args::iterator					Args::begin()
	{
		return m_values.begin();
	}
	Args::iterator					Args::end()
	{
		return m_values.end();
	}
	Args::const_iterator			Args::begin() const
	{
		return m_values.begin();
	}
	Args::const_iterator			Args::end() const
	{
		return m_values.end();
	}
	Args::const_iterator			Args::cbegin() const
	{
		return m_values.cbegin();
	}
	Args::const_iterator			Args::cend() const
	{
		return m_values.cend();
	}
	Args::reverse_iterator			Args::rbegin()
	{
		return m_values.rbegin();
	}
	Args::reverse_iterator			Args::rend()
	{
		return m_values.rend();
	}
	Args::const_reverse_iterator	Args::crbegin() const
	{
		return m_values.crbegin();
	}
	Args::const_reverse_iterator	Args::crend() const
	{
		return m_values.crend();
	}
	
	
	bool Args::equals(const self_type & value) const
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

	bool Args::lessThan(const self_type & value) const
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
}