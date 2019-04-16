#include <MemeCore/File.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	File::File()
		: m_data()
		, m_path()
	{
	}

	File::File(size_t count, CString const * data)
		: m_data()
		, m_path()
	{
		for (size_t i = 0; i < count; i++)
		{
			String line(data[i]);
			line = String::ReplaceAll(line, "\0", "");
			m_data.insert(end(), line.begin(), line.end());
		}
		m_data.push_back('\0');
	}

	File::File(const String & data)
		: m_data(data.begin(), data.end())
		, m_path()
	{
	}

	File::File(const Data & data)
		: m_data(data)
		, m_path()
	{
	}

	File::File(const File & copy)
		: m_data(copy.m_data)
		, m_path(copy.m_path)
	{
	}

	File::~File()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool File::dispose()
	{
		m_data.clear();
		return m_data.empty();
	}

	bool File::loadFromFile(const String & path)
	{
		std::ifstream file((m_path = path), std::ios_base::binary);
		if (file)
		{
			file >> (*this);
			file.close();
			return true;
		}
		return false;
	}

	bool File::loadFromFile()
	{
		return loadFromFile(m_path);
	}

	bool File::saveToFile(const String & filename) const
	{
		std::ofstream file(filename, std::ios_base::binary);
		if (file)
		{
			file << (*this);
			file.close();
			return true;
		}
		return false;
	}

	bool File::saveToFile() const
	{
		return saveToFile(m_path);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void File::serialize(std::ostream & out) const
	{
		out << String(begin(), end());
	}

	void File::deserialize(std::istream & file)
	{
		m_data.clear();

		file.seekg(0, std::ios_base::end);

		std::streamsize size;
		if ((size = file.tellg()) > 0)
		{
			file.seekg(0, std::ios_base::beg);

			m_data.resize(static_cast<size_t>(size));

			file.read(&m_data[0], size);
		}

		m_data.push_back('\0');
	}

	/* * * * * * * * * * * * * * * * * * * * */
}