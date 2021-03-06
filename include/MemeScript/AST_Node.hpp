#ifndef _ML_AST_NODE_HPP_
#define _ML_AST_NODE_HPP_

#include <MemeScript/Export.hpp>
#include <MemeCore/List.hpp>
#include <MemeCore/Preprocessor.hpp>
#include <MemeCore/ITrackable.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_assert_node(derived) ML_assert_is_base_of(ml::AST_Node, derived)

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_SCRIPT_API AST_Node
		: public ITrackable
	{
	public:
		using value_type			= typename List<AST_Node *>::value_type;
		using iterator				= typename List<AST_Node *>::iterator;
		using const_iterator		= typename List<AST_Node *>::const_iterator;
		using reverse_iterator		= typename List<AST_Node *>::reverse_iterator;
		using const_reverse_iterator= typename List<AST_Node *>::const_reverse_iterator;
		using difference_type		= typename List<AST_Node *>::difference_type;

	public:
		AST_Node();
		AST_Node(const List<AST_Node *>& children);
		AST_Node(const AST_Node& copy);
		virtual ~AST_Node();

	public:
		const List<AST_Node *> & getChildren() const;
		const List<AST_Node *> & getSiblings() const;

	public:
		void	clearChildren();
		void	cleanChildren();

	public:
		bool	empty() const;
		bool	contains(AST_Node * node) const;

	public:
		size_t	getDepth() const;
		size_t	childCount() const;
		size_t	siblingCount() const;
		size_t	siblingIndex() const;

	public:
		AST_Node * addChild(AST_Node * value);
		AST_Node * getChild(size_t index) const;
		AST_Node * getParent() const;
		AST_Node * getSibling(size_t index) const;
		AST_Node * push_front(AST_Node * value);
		AST_Node * insertChild(size_t index, AST_Node * value);
		AST_Node * insertChildAfter(AST_Node * pos, AST_Node * value);
		AST_Node * insertChildBefore(AST_Node * pos, AST_Node * value);
		AST_Node * getNext() const;
		AST_Node * getPrev() const;
		AST_Node * setParent(AST_Node * value);

	public:
		inline iterator					begin()				{ return m_children.begin();   }
		inline iterator					end()				{ return m_children.end();	   }
		inline const_iterator			begin()		const	{ return m_children.begin();   }
		inline const_iterator			end()		const	{ return m_children.end();	   }
		inline const_iterator			cbegin()	const	{ return m_children.cbegin();  }
		inline const_iterator			cend()		const	{ return m_children.cend();	   }
		inline reverse_iterator			rbegin()			{ return m_children.rbegin();  }
		inline reverse_iterator			rend()				{ return m_children.rend();	   }
		inline const_reverse_iterator	crbegin()	const	{ return m_children.crbegin(); }
		inline const_reverse_iterator	crend()		const	{ return m_children.crend();   }

		inline const_iterator find(const value_type & value) const
		{
			return std::find(begin(), end(), value);
		}

	public:
		inline void serialize(std::ostream & out) const override
		{
			display(out);
		}
		inline void deserialize(std::istream & in) override
		{
		}
		
		virtual std::ostream & display(std::ostream & out) const;
		
	public:
		int32_t	getID() const;
		
		virtual bool run();
		
		bool runNext();

	public: // Templates
		template <
			class Node
		> inline const Node * as() const
		{
			ML_assert_node(Node);
			return dynamic_cast<const Node *>(this);
		}
		
		template <
			class Node
		> inline Node * as()
		{
			ML_assert_node(Node);
			return dynamic_cast<Node *>(this);
		}

		template <
			class Node
		> Node * nextAs() const
		{
			ML_assert_node(Node);
			if (AST_Node * n = getNext())
			{
				return n->as<Node>();
			}
			return NULL;
		}

		template <
			class Node
		> Node * prevAs() const
		{
			ML_assert_node(Node);
			if (AST_Node * p = getPrev())
			{
				return p->as<Node>();
			}
			return NULL;
		}

		template <
			class Node
		> Node * parentAs() const
		{
			ML_assert_node(Node);
			if (AST_Node * p = getParent())
			{
				return p->as<Node>();
			}
			return NULL;
		}

		template <
			class Node
		> Node * childAs(size_t index) const
		{
			ML_assert_node(Node);
			if (AST_Node * c = getChild(index))
			{
				return c->as<Node>();
			}
			return NULL;
		}

		template <
			class Node
		> Node * firstAs() const
		{
			ML_assert_node(Node);
			if (!empty())
			{
				return (*begin())->as<Node>();
			}
			return NULL;
		}

		template <
			class Node
		> Node * lastAs() const
		{
			ML_assert_node(Node);
			if (!empty())
			{
				return (*end() - 1)->as<Node>();
			}
			return NULL;
		}

	private:
		AST_Node *		m_parent;
		List<AST_Node *>m_children;
		int32_t			m_id;
		static int32_t	s_id;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_AST_NODE_HPP_