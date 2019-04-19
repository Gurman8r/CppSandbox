#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <MemeEngine/Export.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Resources;

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> class Registry final
		: public ITrackable
		, public INonCopyable
	{
		friend class Resources;

		// Type must derive IReadable
		static_assert(std::is_base_of<ml::IReadable, T>::value, "Type must derive ml::IReadable");

	public:
		using value_type	= typename T;
		using pointer		= typename value_type * ;
		using const_pointer = typename const value_type *;
		using PointerMap	= typename HashMap<String, pointer>;
		using FileMap		= typename HashMap<String, String>;
		using Pair			= typename Pair<String, pointer>;
		using iterator		= typename PointerMap::iterator;
		using const_iterator= typename PointerMap::const_iterator;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		Registry(String && name)
			: m_name(name)
			, m_data()
			, m_files()
		{
		}
		~Registry() {}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
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

		inline bool empty() const
		{
			return m_data.empty();
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

		inline const size_t size() const
		{
			return m_data.size();
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
			return ((name)
				? ((it = find(name)) != end())
					? (it->second)
					: (NULL)
				: (NULL));
		}

		inline pointer get(const String & name)
		{
			iterator it;
			return ((name)
				? ((it = find(name)) != end())
					? (it->second)
					: (NULL)
				: (NULL));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline const String getFile(const String & value) const
		{
			FileMap::const_iterator it;
			return ((it = m_files.find(value)) != m_files.end())
				? it->second
				: String();
		}

		inline const List<String> getKeys() const
		{
			List<String> keys;
			for (auto pair : (*this))
			{
				keys.push_back(pair.first);
			}
			return keys;
		}

		inline const String & getName() const
		{
			return m_name;
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool load(const String & name, pointer value)
		{
			if (!value)
			{
				return load(name);
			}
			else if (value && (name && !get(name)))
			{
				return set(name, value);
			}
			else
			{
				return NULL;
			}
		}

		inline pointer load(const String & name, const String & file)
		{
			if (name && !get(name))
			{
				if (m_files[name] = file)
				{
					if (const String path = ML_FS.pathTo(file))
					{
						pointer temp = new value_type();

						if (temp->loadFromFile(path))
						{
							return set(name, temp);
						}
						else
						{
							delete temp;
							Debug::logError("Failed loading {0}: \'{1}\'", name, path);
						}
					}
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
			return ((get(name)) ? (NULL) : (set(name, new value_type())));
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline size_t reload()
		{
			size_t count = 0;
			for (FileMap::const_iterator it = m_files.cbegin(); it != m_files.end(); it++)
			{
				const String & name = it->first;
				const String & file = it->second;
				if (name && file)
				{
					if (const String path = ML_FS.pathTo(file))
					{
						if (pointer temp = get(name))
						{
							count += ((temp->loadFromFile(path))
								? (1U)
								: (Debug::logError("Failed reloading {0}: \'{1}\'", name, path))
							);
						}
					}
				}
			}
			return count;
		}

		inline pointer set(const String & name, pointer value)
		{
			m_data[name] = value;
			return get(name);
		}
		
		/* * * * * * * * * * * * * * * * * * * * */

		inline const_iterator iterAt(const int32_t value) const
		{
			if ((value >= 0) && ((size_t)value < this->size()))
			{
				const_iterator it = this->cbegin();
				for (int32_t i = 0; i < value; i++)
				{
					if ((++it) == this->cend())
					{
						return it;
					}
				}
				return it;
			}
			return this->cend();
		}

		inline const_pointer atIndex(const int32_t value) const
		{
			const_iterator it;
			return (((it = this->iterAt(value)) != this->end())
				? (it->second)
				: (NULL));
		}

		inline const int32_t indexOf(const_pointer value) const
		{
			int32_t index = 0;
			for (auto it = this->cbegin(); it != this->cend(); it++)
			{
				if (it->second == value)
				{
					return index;
				}
				index++;
			}
			return (index = (-1));
		}

		/* * * * * * * * * * * * * * * * * * * * */

	public: // Iterators
		/* * * * * * * * * * * * * * * * * * * * */
		inline iterator			begin()			{ return m_data.begin();  }
		inline iterator			end()			{ return m_data.end();	  }
		inline const_iterator	begin()  const	{ return m_data.begin();  }
		inline const_iterator	end()	 const	{ return m_data.end();	  }
		inline const_iterator	cbegin() const	{ return m_data.cbegin(); }
		inline const_iterator	cend()	 const	{ return m_data.cend();	  }

	private: // Data
		/* * * * * * * * * * * * * * * * * * * * */
		String		m_name;
		PointerMap	m_data;
		FileMap		m_files;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_