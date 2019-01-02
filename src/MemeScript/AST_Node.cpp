#include <MemeScript/AST_Node.h>

namespace ml
{
	int AST_Node::s_id = 0;

	AST_Node::AST_Node()
		: m_parent(NULL)
		, m_children()
		, m_id(s_id++)
	{
	}

	AST_Node::AST_Node(const list_type & children)
		: m_parent(NULL)
		, m_children(children)
		, m_id(s_id++)
	{
	}

	AST_Node::AST_Node(const AST_Node & copy)
		: m_parent(copy.m_parent)
		, m_children(copy.m_children)
		, m_id(s_id++)
	{
	}

	AST_Node::~AST_Node()
	{
		clean();
		clear();
	}


	const AST_Node::list_type& AST_Node::getChildren() const
	{
		return m_children;
	}

	const AST_Node::list_type& AST_Node::getSiblings() const
	{
		static list_type empty;

		if (AST_Node * n = getParent())
		{
			return n->getChildren();
		}

		return empty;
	}


	void AST_Node::clear()
	{
		m_children.clear();
	}

	void AST_Node::clean()
	{
		for (AST_Node * child : m_children)
		{
			delete child;
		}
	}


	bool		AST_Node::empty() const
	{
		return m_children.empty();
	}

	bool		AST_Node::isChildOf(AST_Node * node) const
	{
		return false;
	}

	bool		AST_Node::isParentOf(AST_Node * node) const
	{
		return find(node) != end();
	}


	uint32_t	AST_Node::getDepth() const
	{
		return m_depth;
	}

	AST_Node&	AST_Node::setDepth(uint32_t value)
	{
		m_depth = value;
		return (*this);
	}

	uint32_t	AST_Node::childCount() const
	{
		return m_children.size();
	}

	uint32_t	AST_Node::siblingCount() const
	{
		return getSiblings().size();
	}

	uint32_t	AST_Node::siblingIndex() const
	{
		if (AST_Node * p = getParent())
		{
			uint32_t i = 0;

			for (AST_Node * c : (*p))
			{
				if (c == this)
				{
					return i;
				}

				i++;
			}
		}

		return 0;
	}


	AST_Node * AST_Node::addChild(AST_Node * value)
	{
		if (value)
		{
			value->setParent(this);
			value->setDepth(getDepth() + 1);
			m_children.push_back(value);
			return value;
		}
		return NULL;
	}

	AST_Node * AST_Node::getChild(uint32_t index) const
	{
		if (index < childCount())
		{
			return m_children.at(index);
		}
		return NULL;
	}

	AST_Node * AST_Node::getParent() const
	{
		return m_parent;
	}

	AST_Node * AST_Node::getSibling(uint32_t index) const
	{
		list_type siblings = getSiblings();
		if (index < siblings.size())
		{
			return siblings[index];
		}
		return NULL;
	}

	AST_Node * AST_Node::insertChild(uint32_t index, AST_Node * value)
	{
		if (value)
		{
			value->setParent(this);
			value->setDepth(getDepth() + 1);
			m_children.insert(begin() + index, value);
			return value;
		}
		return NULL;
	}

	AST_Node * AST_Node::insertChildAfter(AST_Node * pos, AST_Node * value)
	{
		if (value)
		{
			AST_Node::const_iterator it = find(pos);
			if (it != end())
			{
				value->setParent(this);
				value->setDepth(getDepth() + 1);
				m_children.insert(it + 1, value);
				return value;
			}
		}
		return NULL;
	}

	AST_Node * AST_Node::insertChildBefore(AST_Node * pos, AST_Node * value)
	{
		if (value)
		{
			AST_Node::const_iterator it = find(pos);
			if (it != end())
			{
				value->setParent(this);
				value->setDepth(getDepth() + 1);
				m_children.insert(it, value);
			}
			return value;
		}
		return NULL;
	}

	AST_Node * AST_Node::getNext() const
	{
		uint32_t i = siblingIndex() + 1;
		if (i < siblingCount())
		{
			return getSibling(i);
		}
		return NULL;
	}

	AST_Node * AST_Node::getPrev() const
	{
		uint32_t i = siblingIndex();
		if (i > 0)
		{
			return getSibling(i - 1);
		}
		return NULL;
	}

	AST_Node * AST_Node::setParent(AST_Node * value)
	{
		m_parent = value;

		return m_parent;
	}


	// Iterators
	AST_Node::iterator			AST_Node::begin()
	{
		return m_children.begin();
	}

	AST_Node::iterator			AST_Node::end()
	{
		return m_children.end();
	}

	AST_Node::const_iterator	AST_Node::begin() const
	{
		return m_children.begin();
	}

	AST_Node::const_iterator	AST_Node::end() const
	{
		return m_children.end();
	}

	AST_Node::const_iterator	AST_Node::cbegin() const
	{
		return m_children.cbegin();
	}

	AST_Node::const_iterator	AST_Node::cend() const
	{
		return m_children.cend();
	}

	AST_Node::reverse_iterator	AST_Node::rbegin()
	{
		return m_children.rbegin();
	}

	AST_Node::reverse_iterator	AST_Node::rend()
	{
		return m_children.rend();
	}

	AST_Node::const_reverse_iterator	AST_Node::crbegin() const
	{
		return m_children.crbegin();
	}

	AST_Node::const_reverse_iterator	AST_Node::crend() const
	{
		return m_children.crend();
	}


	// Search
	AST_Node::const_iterator	AST_Node::find(const value_type & value, uint32_t index) const
	{
		if (value)
		{
			return std::find(begin() + index, end(), value);
		}

		return end();
	}

	AST_Node::const_iterator	AST_Node::find_first(const value_type & value) const
	{
		if (value)
		{
			for (auto it = begin(); it != end(); it++)
			{
				if (*it == value)
				{
					return it;
				}
			}
		}

		return end();
	}

	AST_Node::const_iterator	AST_Node::find_first_not_of(const value_type & value, uint32_t index) const
	{
		if (value)
		{
			for (auto it = begin(); it != end(); it++)
			{
				if (*it != value)
				{
					return it;
				}
			}
		}
		return end();
	}

	AST_Node::const_iterator	AST_Node::find_last(const value_type & value) const
	{
		if (value)
		{
			for (auto it = end(); it != begin(); it--)
			{
				if (*it == value)
				{
					return it;
				}
			}
		}
		return end();
	}

	AST_Node::const_iterator	AST_Node::find_last_not_of(const value_type & value) const
	{
		if (value)
		{
			for (auto it = end(); it != begin(); it--)
			{
				if (*it != value)
				{
					return it;
				}
			}
		}
		return end();
	}


	// Operators
	std::ostream&	operator<<(std::ostream & out, const AST_Node & rhs)
	{
		return rhs.display(out);
	}

	std::ostream&	AST_Node::display(std::ostream & out) const
	{
		for (AST_Node * n : (*this))
		{
			n->display(out);
		}

		return out;
	}


	// Execution
	int		AST_Node::getID() const
	{
		return m_id;
	}

	bool	AST_Node::run()
	{
		return runNext();
	}

	bool	AST_Node::runNext()
	{
		if (AST_Node * n = getNext())
		{
			return n->run();
		}
		return true;
	}
}