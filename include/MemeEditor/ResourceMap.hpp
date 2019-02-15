#ifndef _RESOURCE_MAP_HPP_
#define _RESOURCE_MAP_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IResource.hpp>

namespace ml
{
	template <class T>
	class ResourceMap final
		: public ITrackable
	{
		static_assert(
			std::is_base_of<ml::IResource, T>::value, 
			"Type must derive ml::IResource");

	public:
		using value_type	= T;
		using pointer		= typename value_type * ;
		using const_pointer = typename const pointer ;
		using data_map		= std::unordered_map<String, pointer>;
		using file_map		= std::unordered_map<String, String>;
		using iterator		= typename data_map::iterator;
		using const_iterator= typename data_map::const_iterator;

	public:
		ResourceMap() : m_data() {}

		~ResourceMap() { clear(); }

	public:
		inline pointer load(const String & name)
		{
			if (!find(name))
			{
				m_data[name] = new value_type();
				return find(name);
			}
			return NULL;
		}

		inline pointer load(const String & name, const String & filename)
		{
			if (!name.empty() && !filename.empty() && !find(name))
			{
				pointer temp = new value_type();
				if (temp->loadFromFile(filename))
				{
					m_data[name] = temp;
					return find(name);
				}
				delete temp;
			}
			return NULL;
		}

		inline size_t load(const file_map & files)
		{
			size_t count = 0;
			for (file_map::const_iterator it = files.cbegin(); it != files.end(); it++)
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

		inline pointer find(const String & name)
		{
			iterator it;
			return ((it = m_data.find(name)) != m_data.end())
				? it->second
				: NULL;
		}

		inline const_pointer find(const String & name) const
		{
			const_iterator it;
			return ((it = m_data.find(name)) != m_data.end())
				? it->second
				: NULL;
		}

		inline const data_map & getAll() const
		{
			return m_data;
		}

	private:
		data_map m_data;
	};
}

#endif // !_RESOURCE_MAP_HPP_