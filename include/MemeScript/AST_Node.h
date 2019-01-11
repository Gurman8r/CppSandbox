#ifndef _AST_NODE_H_
#define _AST_NODE_H_

#include <MemeScript/Export.h>
#include <MemeCore/ITrackable.h>
#include <vector>

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

		void	clear();
		void	clean();

		bool	empty() const;
		bool	isChildOf(AST_Node* node) const;
		bool	isParentOf(AST_Node* node) const;

		std::size_t	getDepth() const;

		std::size_t	childCount() const;
		std::size_t	siblingCount() const;
		std::size_t	siblingIndex() const;

		AST_Node*	addChild(AST_Node* value);
		AST_Node*	getChild(std::size_t index) const;
		AST_Node*	getParent() const;
		AST_Node*	getSibling(std::size_t index) const;
		AST_Node*	insertChild(std::size_t index, AST_Node* value);
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
		const_iterator	find(const value_type& value, std::size_t index = 0) const;
		const_iterator	find_first(const value_type& value) const;
		const_iterator	find_first_not_of(const value_type& value, std::size_t index = 0) const;
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

	public:
		template <class T>
		inline const T* As() const
		{
			return dynamic_cast<const T*>(this);
		}
		
		template <class T>
		inline T * As()
		{
			return dynamic_cast<T*>(this);
		}
		
		template <class T>
		inline const bool Is() const
		{
			return bool(As<T>());
		}

	public: // Templates
		template <typename T>
		T* nextAs() const
		{
			if (AST_Node* n = getNext())
			{
				return n->As<T>();
			}
			return NULL;
		}

		template <typename T>
		T* prevAs() const
		{
			if (AST_Node* p = getPrev())
			{
				return p->As<T>();
			}
			return NULL;
		}

		template <typename T>
		T* parentAs() const
		{
			if (AST_Node* p = getParent())
			{
				return p->As<T>();
			}
			return NULL;
		}

		template <typename T>
		T* childAs(std::size_t index) const
		{
			if (AST_Node* c = getChild(index))
			{
				return c->As<T>();
			}
			return NULL;
		}

		template <typename T>
		T* firstAs() const
		{
			if (!empty())
			{
				return (*begin())->As<T>();
			}
			return NULL;
		}

		template <typename T>
		T* lastAs() const
		{
			if (!empty())
			{
				return (*end() - 1)->As<T>();
			}
			return NULL;
		}

	private: // Variables
		AST_Node*	m_parent;
		list_type	m_children;
		int32_t			m_id;

		static int32_t s_id;
	};
}

#endif // !_AST_NODE_H_
