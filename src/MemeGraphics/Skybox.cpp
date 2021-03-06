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
		dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	bool Skybox::dispose()
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
		if (ML_FS.getFileContents(filename, file))
		{
			file >> (*this);
			return true;
		}
		return false;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	void Skybox::serialize(std::ostream & out) const
	{
		out << "U: " << (m_faces[Up]	? (m_faces[Up]->ToString())		: "Nothing") << endl
			<< "D: " << (m_faces[Down]	? (m_faces[Down]->ToString())	: "Nothing") << endl
			<< "L: " << (m_faces[Left]	? (m_faces[Left]->ToString())	: "Nothing") << endl
			<< "R: " << (m_faces[Right]	? (m_faces[Right]->ToString())	: "Nothing") << endl
			<< "F: " << (m_faces[Front]	? (m_faces[Front]->ToString())	: "Nothing") << endl
			<< "B: " << (m_faces[Back]	? (m_faces[Back]->ToString())	: "Nothing") << endl;
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
		if (uint32_t textureID = ML_GL.genTextures(1))
		{
			ML_GL.bindTexture(GL::TextureCubeMap, textureID);
		
			for (uint32_t i = 0; i < faces.size(); i++)
			{
				Image image;
				if (image.loadFromFile(faces[i]))
				{
					ML_GL.texImage2D(
						GL::CubeMap_Positive_X + i,
						0,
						GL::RGB,
						image.width(),
						image.height(),
						0,
						GL::RGB,
						GL::UnsignedByte,
						&image.pixels()[0]
					);
				}
				else
				{
					cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
				}
			}
		
			ML_GL.texParameter(GL::TextureCubeMap, GL::TexMinFilter, GL::Linear);
			ML_GL.texParameter(GL::TextureCubeMap, GL::TexMagFilter, GL::Linear);
			ML_GL.texParameter(GL::TextureCubeMap, GL::TexWrapS, GL::ClampToEdge);
			ML_GL.texParameter(GL::TextureCubeMap, GL::TexWrapT, GL::ClampToEdge);
			ML_GL.texParameter(GL::TextureCubeMap, GL::TexWrapR, GL::ClampToEdge);
			
			ML_GL.bindTexture(GL::TextureCubeMap, NULL);
		
			return textureID;
		}
		return NULL;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}