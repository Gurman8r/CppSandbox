#include <MemeEditor/Manifest.hpp>
#include <MemeCore/FileSystem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	using FileMap = typename Manifest::FileMap;
	using TypeMap = typename Manifest::TypeMap;

	/* * * * * * * * * * * * * * * * * * * * */

	Manifest::Manifest()
	{
	}

	Manifest::~Manifest()
	{
		cleanup();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Manifest::cleanup()
	{
		return true;
	}

	bool Manifest::loadFromFile(const String & filename)
	{
		SStream ss;
		if (ML_FileSystem.getFileContents(filename, ss))
		{
			ss >> (*this);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Manifest::serialize(std::ostream & out) const
	{
		out << std::left;

		out << "Path: \"" << m_path << "\"" << endl;

		for (const Pair<String, FileMap> & pair : m_data)
		{
			out << pair.first << endl;

			for (const Pair<String, String> & data : pair.second)
			{
				out << "| " << std::setw(10) << data.first << " \"" << data.second << "\"" << endl;
			}

			out << endl;
		}
	}

	void Manifest::deserialize(std::istream & in)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](const String & line, const String & find, SStream & ss)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				ss.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			return false;
		};

		auto loadData = [&](const String & name, SStream & ss)
		{
			String temp;
			ss >> temp;
			ss >> m_data[name][temp];
		};

		/* * * * * * * * * * * * * * * * * * * * */

		String line;
		while (std::getline(in, line))
		{
			if (line.front() == '#')
				continue;

			SStream ss;
			if (parseLine(line, "path ", ss))
			{
				ss >> m_path;
			}
			else if (parseLine(line, "font ", ss))
			{
				loadData("font", ss);
			}
			else if (parseLine(line, "image ", ss))
			{
				loadData("image", ss);
			}
			else if (parseLine(line, "mesh ", ss))
			{
				loadData("mesh", ss);
			}
			else if (parseLine(line, "model ", ss))
			{
				loadData("model", ss);
			}
			else if (parseLine(line, "shader ", ss))
			{
				loadData("shader", ss);
			}
			else if (parseLine(line, "sound ", ss))
			{
				loadData("sound", ss);
			}
			else if (parseLine(line, "sprite ", ss))
			{
				loadData("sprite", ss);
			}
			else if (parseLine(line, "texture ", ss))
			{
				loadData("texture", ss);
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */

	const TypeMap & Manifest::getTypes() const
	{
		return m_data;
	}

	const FileMap & Manifest::getFiles(const String & type) const
	{
		return m_data.at(type);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}