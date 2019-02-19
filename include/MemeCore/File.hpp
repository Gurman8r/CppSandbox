#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <MemeCore/ITrackable.hpp>
#include <MemeCore/IReadable.hpp>
#include <MemeCore/IWritable.hpp>
#include <MemeCore/IStream.hpp>
#include <MemeCore/List.hpp>

namespace ml
{
	class ML_CORE_API File final
		: public ITrackable
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

		bool cleanup() override;
		bool loadFromFile(const String & filename) override;
		bool saveToFile(const String & filename) const override;

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const char &		at(size_t i)const { return m_data[i]; }
		inline const Data &		data()		const { return m_data; }
		
		inline char &	at(size_t i){ return m_data[i]; }
		inline Data &	data()		{ return m_data; }

	public:
		inline const char & operator[](size_t i) const { return at(i); }
		inline char &		operator[](size_t i)	   { return at(i); }

		inline operator Data()	 const { return data(); }
		inline operator String() const { return to_str(); }
		inline operator CString()const { return &at(0); }

	public:
		inline iterator			begin()			{ return m_data.begin();  }
		inline iterator			end()			{ return m_data.end();	  }
		inline const_iterator	begin()  const	{ return m_data.begin();  }
		inline const_iterator	end()	 const	{ return m_data.end();	  }
		inline const_iterator	cbegin() const	{ return m_data.cbegin(); }
		inline const_iterator	cend()	 const	{ return m_data.cend();	  }

	public:
		inline reverse_iterator			rbegin()		{ return m_data.rbegin();  }
		inline reverse_iterator			rend()			{ return m_data.rend();	  }
		inline const_reverse_iterator	rbegin()  const	{ return m_data.rbegin();  }
		inline const_reverse_iterator	rend()	  const	{ return m_data.rend();	  }
		inline const_reverse_iterator	crbegin() const	{ return m_data.crbegin(); }
		inline const_reverse_iterator	crend()	  const	{ return m_data.crend();	  }


		template <class T>
		inline std::istream & operator>>(T & t)
		{
			SStream ss = to_stream();
			ss >> t;
			return ss;
		}

	private:
		Data m_data;
	};
}

#endif // !_FILE_HPP_
