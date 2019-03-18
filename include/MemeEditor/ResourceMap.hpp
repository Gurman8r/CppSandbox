
#ifndef _RESOURCE_MAP_HPP_
#define _RESOURCE_MAP_HPP_

#include <MemeEditor/Export.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ResourceManager;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class _Elem>
	class ResourceMap final
		: public ITrackable
		, public INonCopyable
	{
		static_assert(
			std::is_base_of<ml::IReadable, _Elem>::value, 
			"Type must derive ml::IReadable");

	public:
		using value_type	= _Elem;
		using pointer		= typename value_type * ;
		using const_pointer = typename const pointer ;

		using PointerMap	= HashMap<String, pointer>;
		using FileMap		= HashMap<String, String>;
		using Pair			= Pair<String, pointer>;
		
		using iterator		= typename PointerMap::iterator;
		using const_iterator= typename PointerMap::const_iterator;

	private:
		ResourceMap() {}
		~ResourceMap() {}

		PointerMap	m_data;
		String		m_path;
		FileMap		m_files;

		friend class ResourceManager;

	public:
		inline void serialize(std::ostream & out) const override
		{
			for (auto pair : m_files)
			{
				out << std::left << std::setw(12)
					<< pair.first << " \'" << pair.second << "\'"
					<< endl;
			}
			out << endl;
		}
		inline void deserialize(std::istream & in) override
		{
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */

		inline size_t clean()
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
				m_data.erase(it);
				return !get(name);
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline iterator find(const String & name)
		{
			return m_data.find(name);
		}

		inline const_iterator find(const String & name) const
		{
			return m_data.find(name);
		}

		/* * * * * * * * * * * * * * * * * * * * */

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

		/* * * * * * * * * * * * * * * * * * * * */

		inline const PointerMap & getAll() const
		{
			return m_data;
		}

		inline PointerMap & getAll()
		{
			return m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool load(const FileMap & files)
		{
			return load(files, String());
		}

		inline size_t load(const FileMap & files, const String & path)
		{
			m_files = files;
			m_path = path;

			size_t count = 0;
			for (FileMap::const_iterator it = files.cbegin(); it != files.end(); it++)
			{
				if (const_pointer temp = load(it->first, it->second))
				{
					count++;
				}
			}
			return count;
		}


		inline pointer load(const String & name, const String & file)
		{
			if (!name.empty() && !get(name))
			{
				if (!file.empty())
				{
					pointer temp = new value_type();

					if (temp->loadFromFile(pathTo(file)))
					{
						return set(name, temp);
					}
					
					delete temp;

					Debug::logError("Failed loading {0}: \'{1}\'", name, pathTo(file));

				}
				else
				{
					return load(name);
				}
			}
			return NULL;
		}

		inline pointer load(const String & name)
		{
			return (get(name)
				? NULL
				: set(name, new value_type()));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String pathTo(const String & file) const 
		{ 
			return ML_FileSystem.pathTo(m_path + file); 
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline size_t reload()
		{
			size_t count = 0;
			for (FileMap::const_iterator it = m_files.cbegin(); it != m_files.end(); it++)
			{
				const String & name = it->first;
				const String & file = it->second;

				if (!name.empty() && !file.empty())
				{
					if (pointer temp = get(name))
					{
						if (temp->loadFromFile(pathTo(file)))
						{
							count++;
						}
						else
						{
							Debug::logError("Failed reloading {0}: \'{1}\'", name, pathTo(file));
						}
					}
				}
			}
			return count;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline pointer set(const String & name, pointer value)
		{
			m_data[name] = value;
			return get(name);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline const size_t size() const
		{
			return m_data.size();
		}

		/* * * * * * * * * * * * * * * * * * * * */

	public:
		inline iterator			begin()			{ return m_data.begin();  }
		inline iterator			end()			{ return m_data.end();	  }
		inline const_iterator	begin()  const	{ return m_data.begin();  }
		inline const_iterator	end()	 const	{ return m_data.end();	  }
		inline const_iterator	cbegin() const	{ return m_data.cbegin(); }
		inline const_iterator	cend()	 const	{ return m_data.cend();	  }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_RESOURCE_MAP_HPP_