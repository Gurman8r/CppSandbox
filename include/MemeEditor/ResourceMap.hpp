#ifndef _RESOURCE_MAP_HPP_
#define _RESOURCE_MAP_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	template <class _Elem>
	class ResourceMap final
		: public ITrackable
	{
		static_assert(
			std::is_base_of<ml::IResource, _Elem>::value, 
			"Type must derive ml::IResource");

	public:
		using value_type	= _Elem;
		using pointer		= typename value_type * ;
		using const_pointer = typename const pointer ;

		using PointerMap	= std::unordered_map<String, pointer>;
		using FileMap		= std::unordered_map<String, String>;
		using Pair			= std::pair<String, pointer>;
		
		using iterator		= typename PointerMap::iterator;
		using const_iterator= typename PointerMap::const_iterator;

	public:
		ResourceMap() : m_data() {}

		~ResourceMap() { clear(); }

	public:
		inline pointer assign(const String & name, pointer value)
		{
			m_data[name] = value;
			return get(name);
		}

		inline size_t clear()
		{
			size_t count = 0;
			for (auto it = m_data.begin(); it != m_data.end(); it++)
			{
				if (it->second)
				{
					delete it->second;
					it->second = NULL;
					count++;
				}
			}
			m_data.clear();
			return count;
		}

		inline bool erase(const String & name)
		{
			iterator it;
			if ((it = find(name)) != end())
			{
				delete it->second;
				it->second = NULL;
				m_data.erase(it);
				return !get(name);
			}
			return false;
		}

		inline iterator find(const String & name)
		{
			return m_data.find(name);
		}

		inline const_iterator find(const String & name) const
		{
			return m_data.find(name);
		}

		inline const_pointer get(const String & name) const
		{
			const_iterator it;
			return ((it = find(name)) != end())
				? it->second
				: NULL;
		}

		inline pointer get(const String & name)
		{
			iterator it;
			return ((it = find(name)) != end())
				? it->second
				: NULL;
		}

		inline const PointerMap & getAll() const
		{
			return m_data;
		}

		inline PointerMap & getAll()
		{
			return m_data;
		}

		inline pointer load(const String & name)
		{
			return (get(name)
				? NULL
				: assign(name, new value_type()));
		}

		inline pointer load(const String & name, const String & filename)
		{
			if (!name.empty() && !get(name))
			{
				if (!filename.empty())
				{
					pointer temp = new value_type();
					if (temp->loadFromFile(filename))
					{
						return assign(name, temp);
					}
					delete temp;
				}
				else
				{
					return load(name);
				}
			}
			return NULL;
		}

		inline size_t load(const FileMap & files)
		{
			size_t count = 0;
			for (FileMap::const_iterator it = files.cbegin(); it != files.end(); it++)
			{
				if (const_pointer temp = load(it->first, it->second))
				{
					Debug::log("Loaded | {0} | {1}", it->first, it->second);
					count++;
				}
				else
				{
					Debug::logError("Failed Loading | {0} | {1}", it->first, it->second);
				}
			}
			return count;
		}

	public:
		inline iterator			begin()			{ return m_data.begin();	}
		inline iterator			end()			{ return m_data.end();		}
		inline const_iterator	begin()  const	{ return m_data.begin();	}
		inline const_iterator	end()	 const	{ return m_data.end();		}
		inline const_iterator	cbegin() const	{ return m_data.cbegin();	}
		inline const_iterator	cend()	 const	{ return m_data.cend();		}

	private:
		PointerMap m_data;
	};
}

#endif // !_RESOURCE_MAP_HPP_