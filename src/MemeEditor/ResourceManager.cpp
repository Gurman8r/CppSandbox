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
		return (ML_FileSystem.getFileContents(filename, file) ? parseFile(file) : false);
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

			if (parseItem(file, line))
			{
				if (parseTable(m_manifest.back()))
				{
					count++;
				}
			}
		}
		return (bool)(count);
	}

	bool ResourceManager::parseItem(SStream & file, String & line)
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
					if (parseLine(line, "type:", ss))
					{
						m_manifest.back()["type"] = ss.str();
					}
					else if (parseLine(line, "name:", ss))
					{
						m_manifest.back()["name"] = ss.str();
					}
					else if (parseLine(line, "file:", ss))
					{
						m_manifest.back()["file"] = ss.str();
					}
					else if (parseLine(line, "font:", ss))
					{
						m_manifest.back()["font"] = ss.str();
					}
					else if (parseLine(line, "image:", ss))
					{
						m_manifest.back()["image"] = ss.str();
					}
					else if (parseLine(line, "mesh:", ss))
					{
						m_manifest.back()["mesh"] = ss.str();
					}
					else if (parseLine(line, "model:", ss))
					{
						m_manifest.back()["model"] = ss.str();
					}
					else if (parseLine(line, "shader:", ss))
					{
						m_manifest.back()["shader"] = ss.str();
					}
					else if (parseLine(line, "texture:", ss))
					{
						m_manifest.back()["texture"] = ss.str();
					}
				}
			}
		}

		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	bool ResourceManager::parseTable(const StringTable & data)
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
				if (type == "manifest")
				{
					return loadFromFile(name);
				}
				else if (type == "font")
				{
					if (Font * temp = fonts.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "image")
				{
					if (Image * temp = images.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "material")
				{
					if (Material * temp = mats.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "mesh")
				{
					if (Mesh * temp = meshes.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "model")
				{
					if (Model * temp = models.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "script")
				{
					if (Script * temp = scripts.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "shader")
				{
					if (Shader * temp = shaders.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
						else
						{
							const String vert = getValue(data, "vert");
							const String geom = getValue(data, "geom");
							const String frag = getValue(data, "frag");
							if (vert || geom || frag)
							{
								return temp->loadFromFile(vert, geom, frag);
							}
						}
					}
				}
				else if (type == "skybox")
				{
					if (Skybox * temp = skyboxes.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "sound")
				{
					if (Sound * temp = sounds.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
					}
				}
				else if (type == "sprite")
				{
					if (Sprite * temp = sprites.load(name))
					{
						if (const String file = getValue(data, "texture"))
						{
							if (const Texture * tex = textures.get(file))
							{
								return temp->loadFromMemory(tex)
									? Debug::log("Loaded {0} {1}", type, name)
									: Debug::logError("Failed Loading {0} {1}", type, name);
							}
						}
					}
				}
				else if (type == "texture")
				{
					if (Texture * temp = textures.load(name))
					{
						if (const String file = getValue(data, "file"))
						{
							return temp->loadFromFile(file);
						}
						else if (const String file = getValue(data, "image"))
						{
							if (const Image * img = images.get(file))
							{
								return temp->loadFromImage(*img);
							}
						}
					}
				}
			}
		}
		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}