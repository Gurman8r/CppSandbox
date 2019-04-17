#ifndef _ML_FILE_HPP_
#define _ML_FILE_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IDisposable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/IWritable.hpp>
#include <MemeCore/IStream.hpp>
#include <MemeCore/List.hpp>

namespace ml
{
	class ML_CORE_API File final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public IWritable
	{
	public:
		using Data = List<char>;

		using iterator				= typename Data::iterator;
		using const_iterator		= typename Data::const_iterator;
		using reverse_iterator		= typename Data::reverse_iterator;
		using const_reverse_iterator= typename Data::const_reverse_iterator;

	public:
		File();
		File(size_t count, CString const * data);
		File(const String & data);
		File(const Data & data);
		File(const File & copy);
		~File();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile();
		bool saveToFile(const String & filename) const override;
		bool saveToFile() const;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const char &		at(size_t i) const	{ return m_data[i]; }
		inline char &			at(size_t i)		{ return m_data[i]; }
		inline const Data &		data() const		{ return m_data;	}
		inline Data &			data()				{ return m_data;	}
		inline const String &	path() const		{ return m_path;	}

	public:
		inline const char & operator[](size_t i) const { return at(i); }
		inline char &		operator[](size_t i)	   { return at(i); }

		inline operator bool		() const { return !m_data.empty(); }
		inline operator List<char>	() const { return data(); }
		inline operator String		() const { return ToString(); }
		inline operator CString		() const { return &at(0); }

		inline File & operator=(const String & value)
		{
			return (*this) = File(value);
		}

	public:
		inline iterator					begin()			{ return m_data.begin();	}
		inline iterator					end()			{ return m_data.end();		}
		inline const_iterator			begin()  const	{ return m_data.begin();	}
		inline const_iterator			end()	 const	{ return m_data.end();		}
		inline const_iterator			cbegin() const	{ return m_data.cbegin();	}
		inline const_iterator			cend()	 const	{ return m_data.cend();		}
		inline reverse_iterator			rbegin()		{ return m_data.rbegin();	}
		inline reverse_iterator			rend()			{ return m_data.rend();		}
		inline const_reverse_iterator	rbegin()  const	{ return m_data.rbegin();	}
		inline const_reverse_iterator	rend()	  const	{ return m_data.rend();		}
		inline const_reverse_iterator	crbegin() const	{ return m_data.crbegin();	}
		inline const_reverse_iterator	crend()	  const	{ return m_data.crend();	}

	private:
		String	m_path;
		Data	m_data;
	};
}

#endif // !_ML_FILE_HPP_