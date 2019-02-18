#include <MemeGraphics/Skybox.hpp>
#include <MemeCore/FileSystem.hpp>
#include <MemeCore/Debug.hpp>
#include <MemeGraphics/OpenGL.hpp>

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
			uint32_t cur = MAX_FACE;
			switch (line.front())
			{
			case '#': break;
			case 'U': cur = Up;		break;
			case 'D': cur = Down;	break;
			case 'L': cur = Left;	break;
			case 'R': cur = Right;	break;
			case 'F': cur = Front;	break;
			case 'B': cur = Back;	break;
			default:
				SStream ss;
				if (parseLine(line, "path: ", ss))
				{
					ss >> path;
				}
				break;
			}

			if (cur == MAX_FACE)
				continue;

			const String type(String(1, line.front()) + ": ");

			SStream ss;
			if (parseLine(line, type, ss))
			{
				ss >> line;

				const String file = (path + line);

				if (!m_faces[cur] && (m_faces[cur] = new Texture()))
				{
					if (!m_faces[cur]->loadFromFile(file, GL::TextureCubeMap))
					{
						Debug::logError("Failed Loading Skybox Face {0}: \'{1}\'", cur, file);
						delete m_faces[cur];
						m_faces[cur] = NULL;
					}
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	uint32_t Skybox::loadCubemap(const List<String>& faces)
	{
		if (uint32_t textureID = OpenGL::genTextures(1))
		{
			OpenGL::bindTexture(GL::TextureCubeMap, textureID);

			for (uint32_t i = 0; i < faces.size(); i++)
			{
				Image image;
				if (image.loadFromFile(faces[i]))
				{
					OpenGL::texImage2D(
						GL::CubeMap_Positive_X + i,
						0,
						GL::RGB,
						image.width(),
						image.height(),
						0,
						GL::RGB,
						GL::UnsignedByte,
						image.ptr()
					);
				}
				else
				{
					cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
				}
			}

			OpenGL::texParameter(GL::TextureCubeMap, GL::TexMinFilter, GL::Linear);
			OpenGL::texParameter(GL::TextureCubeMap, GL::TexMagFilter, GL::Linear);
			OpenGL::texParameter(GL::TextureCubeMap, GL::TexWrapS, GL::ClampToEdge);
			OpenGL::texParameter(GL::TextureCubeMap, GL::TexWrapT, GL::ClampToEdge);
			OpenGL::texParameter(GL::TextureCubeMap, GL::TexWrapR, GL::ClampToEdge);
			
			OpenGL::bindTexture(GL::TextureCubeMap, NULL);

			return textureID;
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}