#ifndef _AST_NODE_HPP_
#define _AST_NODE_HPP_

#include <MemeScript/Export.hpp>
#include <MemeCore/ITrackable.hpp>

#define assert_typeof_node(T) \
static_assert(std::is_base_of<ml::AST_Node, T>::value, "Type must derive ml::AST_Node") \

namespace ml
{
	class ML_SCRIPT_API AST_Node
		: public ITrackable
	{
	public:
		using list_type				= std::vector<AST_Node*>;
		using value_type			= list_type::value_type;
		using iterator				= list_type::iterator;
		using const_iterator		= list_type::const_iterator;
		using reverse_iterator		= list_type::reverse_iterator;
		using const_reverse_iterator= list_type::const_reverse_iterator;
		using difference_type		= list_type::difference_type;

	public:
		AST_Node();
		AST_Node(const list_type& children);
		AST_Node(const AST_Node& copy);
		virtual ~AST_Node();

		const list_type& getChildren() const;
		const list_type& getSiblings() const;

		void	clearChildren();
		void	cleanChildren();

		bool	empty() const;
		bool	isChildOf(AST_Node* node) const;
		bool	isParentOf(AST_Node* node) const;

		size_t	getDepth() const;

		size_t	childCount() const;
		size_t	siblingCount() const;
		size_t	siblingIndex() const;

		AST_Node*	addChild(AST_Node* value);
		AST_Node*	getChild(size_t index) const;
		AST_Node*	getParent() const;
		AST_Node*	getSibling(size_t index) const;
		AST_Node*	insertChild(size_t index, AST_Node* value);
		AST_Node*	insertChildAfter(AST_Node* pos, AST_Node* value);
		AST_Node*	insertChildBefore(AST_Node* pos, AST_Node* value);
		AST_Node*	getNext() const;
		AST_Node*	getPrev() const;
		AST_Node*	setParent(AST_Node* value);

	public: // Iterators
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

	public: // Search
		const_iterator	find(const value_type& value, size_t index = 0) const;
		const_iterator	find_first(const value_type& value) const;
		const_iterator	find_first_not_of(const value_type& value, size_t index = 0) const;
		const_iterator	find_last(const value_type& value) const;
		const_iterator	find_last_not_of(const value_type& value) const;

	public: // Operators
		inline void serialize(std::ostream & out) const
		{
			display(out);
		}
		virtual std::ostream& display(std::ostream& out) const;
		
	public: // Execution
		int32_t	getID() const;
		virtual bool run();
		bool runNext();

	public: // Templates
		template <class T>
		inline const T * as() const
		{
			assert_typeof_node(T);
			return dynamic_cast<const T *>(this);
		}
		
		template <class T>
		inline T * as()
		{
			assert_typeof_node(T);
			return dynamic_cast<T *>(this);
		}

		template <class T>
		T * nextAs() const
		{
			assert_typeof_node(T);
			if (AST_Node* n = getNext())
			{
				return n->as<T>();
			}
			return NULL;
		}

		template <class T>
		T * prevAs() const
		{
			assert_typeof_node(T);
			if (AST_Node* p = getPrev())
			{
				return p->as<T>();
			}
			return NULL;
		}

		template <class T>
		T * parentAs() const
		{
			assert_typeof_node(T);
			if (AST_Node* p = getParent())
			{
				return p->as<T>();
			}
			return NULL;
		}

		template <class T>
		T * childAs(size_t index) const
		{
			assert_typeof_node(T);
			if (AST_Node* c = getChild(index))
			{
				return c->as<T>();
			}
			return NULL;
		}

		template <class T>
		T * firstAs() const
		{
			assert_typeof_node(T);
			if (!empty())
			{
				return (*begin())->as<T>();
			}
			return NULL;
		}

		template <class T>
		T * lastAs() const
		{
			assert_typeof_node(T);
			if (!empty())
			{
				return (*end() - 1)->as<T>();
			}
			return NULL;
		}

	private: // Variables
		AST_Node *	m_parent;
		list_type	m_children;
		int32_t		m_id;

		static int32_t s_id;
	};
}

#endif // !_AST_NODE_HPP_
