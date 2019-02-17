#include <MemeGraphics/Skybox.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Skybox::Skybox()
	{
	}

	Skybox::~Skybox()
	{
		cleanup();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	bool Skybox::cleanup()
	{
		for (Texture * t : m_faces)
		{
			if (t) { delete t; }
		}
		return true;
	}
	
	bool Skybox::loadFromFile(const String & filename)
	{
		SStream file;
		if (ML_FileSystem.getFileContents(filename, file))
		{
			file >> (*this);
			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Skybox::serialize(std::ostream & out) const
	{
		out << "U: " << (m_faces[Up]	? (m_faces[Up]->to_str())	: "Nothing" )<< endl
			<< "D: " << (m_faces[Down]	? (m_faces[Down]->to_str())	: "Nothing" )<< endl
			<< "L: " << (m_faces[Left]	? (m_faces[Left]->to_str())	: "Nothing" )<< endl
			<< "R: " << (m_faces[Right]	? (m_faces[Right]->to_str()): "Nothing" )<< endl
			<< "F: " << (m_faces[Front]	? (m_faces[Front]->to_str()): "Nothing" )<< endl
			<< "B: " << (m_faces[Back]	? (m_faces[Back]->to_str())	: "Nothing" )<< endl;
	}
	
	void Skybox::deserialize(std::istream & in)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](
			const String & line,
			const String & find,
			SStream & ss)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				ss.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			return false;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		String path = String();

		String line;
		while (std::getline(in, line))
		{
			SStream pathSs;
			if (parseLine(line, "path: ", pathSs))
			{
				pathSs >> path;
				continue;
			}

			uint32_t current = MAX_FACE;
			switch (line.front())
			{
			case 'U': current = Up;		break;
			case 'D': current = Down;	break;
			case 'L': current = Left;	break;
			case 'R': current = Right;	break;
			case 'F': current = Front;	break;
			case 'B': current = Back;	break;
			}

			if (current == MAX_FACE)
				continue;

			const String type(String(1, line.front()) + ": ");

			SStream ss;
			if (parseLine(line, type, ss))
			{
				ss >> line;

				const String file = (path + line);

				if (!m_faces[current])
				{
					m_faces[current] = new Texture();

					if (!m_faces[current]->loadFromFile(file))
					{
						Debug::logError("Failed Loading Skybox Face {0}: \'{1}\'", 
							current,
							file);

						delete m_faces[current];
						m_faces[current] = NULL;
					}
				}
			}
		}
		
		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Skybox::draw(RenderTarget & target, RenderBatch batch) const
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */
}