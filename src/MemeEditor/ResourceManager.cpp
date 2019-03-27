#include <MemeEditor/ResourceManager.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager() 
	{
		cleanAll();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t ResourceManager::cleanAll()
	{
		return
			sounds.clean() +
			sprites.clean() +
			models.clean() +
			meshes.clean() +
			mats.clean() +
			shaders.clean() +
			skyboxes.clean() +
			textures.clean() +
			images.clean() +
			fonts.clean() +
			scripts.clean();
	}

	size_t ResourceManager::reloadAll()
	{
		return
			scripts.reload() +
			fonts.reload() +
			images.reload() +
			meshes.reload() +
			models.reload() +
			mats.reload() +
			shaders.reload() +
			skyboxes.reload() +
			sounds.reload() +
			sprites.reload() +
			textures.reload();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ResourceManager::cleanup()
	{
		return cleanAll();
	}

	bool ResourceManager::loadFromFile(const String & filename)
	{
		SStream file;
		return ML_FileSystem.getFileContents(filename, file) && parseFile(file);
	}

	void ResourceManager::serialize(std::ostream & out) const
	{
		out << "Resources:" << endl;

		for (auto data : m_manifest)
		{
			for (auto pair : data)
			{
				out << "[" << pair.first << "] \'" << pair.second << "\'"
					<< endl;
			}
			out << endl;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool ResourceManager::parseFile(SStream & file)
	{
		size_t count = 0;
		String line;
		while (std::getline(file, line))
		{
			line.trim();

			if (line.front() == '#')
				continue;

			if (parseItems(file, line))
			{
				if (parseValue(m_manifest.back()))
				{
					count++;
				}
			}
		}
		return (bool)(count);
	}

	bool ResourceManager::parseItems(SStream & file, String & line)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](const String & line, const String & find, SStream & ss)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				ss.str(String(line.substr((i + find.size()), (line.size() - find.size() - 2))).trim());
				return true;
			}
			return false;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (line.find("<item>") != String::npos)
		{
			m_manifest.push_back(StringTable());

			while (std::getline(file, line))
			{
				if (line.find("</item>") != String::npos)
				{
					return true;
				}
				else
				{
					SStream ss;
					if (parseLine(line,		 "type:",	ss)) { m_manifest.back()["type"]	= ss.str(); }
					else if (parseLine(line, "name:",	ss)) { m_manifest.back()["name"]	= ss.str(); }
					else if (parseLine(line, "file:",	ss)) { m_manifest.back()["file"]	= ss.str(); }
					else if (parseLine(line, "font:",	ss)) { m_manifest.back()["font"]	= ss.str(); }
					else if (parseLine(line, "image:",	ss)) { m_manifest.back()["image"]	= ss.str(); }
					else if (parseLine(line, "mesh:",	ss)) { m_manifest.back()["mesh"]	= ss.str(); }
					else if (parseLine(line, "model:",	ss)) { m_manifest.back()["model"]	= ss.str(); }
					else if (parseLine(line, "shader:",	ss)) { m_manifest.back()["shader"]	= ss.str(); }
					else if (parseLine(line, "texture:",ss)) { m_manifest.back()["texture"]	= ss.str(); }
				}
			}
		}

		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	bool ResourceManager::parseValue(const StringTable & data)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto getValue = [](const StringTable & data, const String & find)
		{
			auto it = data.find(find);
			return ((it != data.end()) ? (it->second) : (String()));
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (const String type = getValue(data, "type"))
		{
			if (const String name = getValue(data, "name"))
			{
				// Manifest
				if (type == "manifest")
				{
					return loadFromFile(name);
				}
				// Font
				else if (type == "font")
				{
					if (const String file = getValue(data, "file"))
					{
						return fonts.load(name, file);
					}
				}
				// Image
				else if (type == "image")
				{
					if (const String file = getValue(data, "file"))
					{
						return images.load(name, file);
					}
				}
				// Material
				else if (type == "material")
				{
					if (const String file = getValue(data, "file"))
					{
						return mats.load(name, file);
					}
				}
				// Mesh
				else if (type == "mesh")
				{
					if (const String file = getValue(data, "file"))
					{
						return meshes.load(name, file);
					}
				}
				// Model
				else if (type == "model")
				{
					if (const String file = getValue(data, "file"))
					{
						return models.load(name, file);
					}
					else if (const String file = getValue(data, "mesh"))
					{
						return
							models.load(name) &&
							models.get(name)->loadFromMemory(*meshes.get(file));
					}
				}
				// Script
				else if (type == "script")
				{
					if (const String file = getValue(data, "file"))
					{
						return scripts.load(name, file);
					}
				}
				// Shader
				else if (type == "shader")
				{
					if (const String file = getValue(data, "file"))
					{
						return shaders.load(name, file);
					}
					else
					{
						const String vert = getValue(data, "vert");
						const String geom = getValue(data, "geom");
						const String frag = getValue(data, "frag");
						if (vert || geom || frag)
						{
							return
								shaders.load(name) &&
								shaders.get(name)->loadFromMemory(
									vert, 
									geom, 
									frag);
						}
					}
				}
				// Skybox
				else if (type == "skybox")
				{
					if (const String file = getValue(data, "file"))
					{
						return skyboxes.load(name, file);
					}
				}
				// Sound
				else if (type == "sound")
				{
					if (const String file = getValue(data, "file"))
					{
						return sounds.load(name, file);
					}
				}
				// Sprite
				else if (type == "sprite")
				{
					if (Sprite * temp = sprites.load(name))
					{
						if (const String file = getValue(data, "texture"))
						{
							return temp->loadFromMemory(textures.get(file));
						}
					}
				}
				// Texture
				else if (type == "texture")
				{
				if (const String file = getValue(data, "file"))
				{
					return textures.load(name, file);
				}
				else if (const String file = getValue(data, "image"))
				{
					return
						textures.load(name) &&
						textures.get(name)->loadFromImage(*images.get(file));
				}
				}
			}
		}
		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}