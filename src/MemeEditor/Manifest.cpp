#include <MemeEditor/Manifest.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
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
		m_files.clear();
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

		out << FG::Green << "path"
			<< FG::White << " | "
			<< FG::Magenta << "\"" << m_path << "\"" 
			<< endl;

		for (auto pair : m_files)
		{
			out << FG::Green << pair.first
				<< FG::White << " | "
				<< FG::Cyan << pair.second.size()
				<< endl;

			for (auto data : pair.second)
			{
				out << FG::Normal << " | " << std::setw(12) 
					<< FG::Yellow << data.first 
					<< FG::Magenta << " \"" << data.second << "\""
					<< endl;
			}
		}
		out << FMT();
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

		auto loadData = [&](const String & type, SStream & ss)
		{
			if (ss.good())
			{
				String name;
				ss >> name;

				if (ss.good())
				{
					ss >> m_files[type][name];
				}
				else
				{
					m_files[type][name] = String();
				}
			}
		};

		/* * * * * * * * * * * * * * * * * * * * */

		String line;
		while (std::getline(in, line))
		{
			if (line.front() == '#')
				continue;

			SStream ss;
			if (parseLine(line, "path:", ss))
			{
				ss >> m_path;
			}
			else if (parseLine(line, "font:",	ss)) { loadData("fonts",	ss); }
			else if (parseLine(line, "image:",	ss)) { loadData("images",	ss); }
			else if (parseLine(line, "mesh:",	ss)) { loadData("meshes",	ss); }
			else if (parseLine(line, "model:",	ss)) { loadData("models",	ss); }
			else if (parseLine(line, "shader:",	ss)) { loadData("shaders",	ss); }
			else if (parseLine(line, "skybox:",	ss)) { loadData("skyboxes",	ss); }
			else if (parseLine(line, "sound:",	ss)) { loadData("sounds",	ss); }
			else if (parseLine(line, "sprite:",	ss)) { loadData("sprites",	ss); }
			else if (parseLine(line, "texture:",ss)) { loadData("textures",	ss); }
		}

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}