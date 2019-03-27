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
			scripts.clean() +
			plugins.clean();
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
			textures.reload() +
			plugins.reload();
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

		auto parseLine = [](const String & l, const String & d, SStream & ss)
		{
			size_t i;
			if ((i = l.find(d)) != String::npos)
			{
				ss.str(String(l.substr((i + d.size()), (l.size() - d.size() - 2))).trim());
				return true;
			}
			return false;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		if (line.find("<item>") != String::npos)
		{
			m_manifest.push_back(HashMap<String, String>());

			while (std::getline(file, line))
			{
				line.replaceAll("%Configuration%", Debug::config());
				line.replaceAll("%PlatformTarget%", Debug::platform());

				if (line.find("</item>") != String::npos)
				{
					return true;
				}
				else
				{
					SStream ss;
					if (parseLine(line, "type:", ss)) { m_manifest.back()["type"] = ss.str(); }
					else if (parseLine(line, "name:", ss)) { m_manifest.back()["name"] = ss.str(); }
					else if (parseLine(line, "file:", ss)) { m_manifest.back()["file"] = ss.str(); }
					else if (parseLine(line, "font:", ss)) { m_manifest.back()["font"] = ss.str(); }
					else if (parseLine(line, "image:", ss)) { m_manifest.back()["image"] = ss.str(); }
					else if (parseLine(line, "mesh:", ss)) { m_manifest.back()["mesh"] = ss.str(); }
					else if (parseLine(line, "model:", ss)) { m_manifest.back()["model"] = ss.str(); }
					else if (parseLine(line, "plugin:", ss)) { m_manifest.back()["plugin"] = ss.str(); }
					else if (parseLine(line, "shader:", ss)) { m_manifest.back()["shader"] = ss.str(); }
					else if (parseLine(line, "texture:", ss)) { m_manifest.back()["texture"] = ss.str(); }
				}
			}
		}

		return false;

		/* * * * * * * * * * * * * * * * * * * * */
	}

	bool ResourceManager::parseValue(const HashMap<String, String> & data)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto getValue = [](const HashMap<String, String> & data, const String & find)
		{
			HashMap<String, String>::const_iterator it;
			return (((it = data.find(find)) != data.end()) 
				? (it->second) 
				: (String()));
		};

		/* * * * * * * * * * * * * * * * * * * * */

		const String type = getValue(data, "type");
		const String name = getValue(data, "name");
		if (type && name)
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
				else
				{
					return fonts.load(name);
				}
			}
			// Image
			else if (type == "image")
			{
				if (const String file = getValue(data, "file"))
				{
					return images.load(name, file);
				}
				else
				{
					return images.load(name);
				}
			}
			// Material
			else if (type == "material")
			{
				if (const String file = getValue(data, "file"))
				{
					return mats.load(name, file);
				}
				else
				{
					return mats.load(name);
				}
			}
			// Mesh
			else if (type == "mesh")
			{
				if (const String file = getValue(data, "file"))
				{
					return meshes.load(name, file);
				}
				else
				{
					return meshes.load(name);
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
				else
				{
					return models.load(name);
				}
			}
			// Plugin
			else if (type == "plugin")
			{
				if (const String file = getValue(data, "file"))
				{
					return plugins.load(name, file);
				}
				else
				{
					return plugins.load(name);
				}
			}
			// Script
			else if (type == "script")
			{
				if (const String file = getValue(data, "file"))
				{
					return scripts.load(name, file);
				}
				else
				{
					return scripts.load(name);
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
							shaders.get(name)->loadFromFile(vert, geom, frag);
					}
					else
					{
						return shaders.load(name);
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
				else
				{
					return skyboxes.load(name);
				}
			}
			// Sound
			else if (type == "sound")
			{
				if (const String file = getValue(data, "file"))
				{
					return sounds.load(name, file);
				}
				else
				{
					return sounds.load(name);
				}
			}
			// Sprite
			else if (type == "sprite")
			{
				if (const String file = getValue(data, "texture"))
				{
					const Texture * tex;
					return
						(sprites.load(name)) &&
						(tex = textures.get(file)) &&
						(sprites.get(name)->loadFromMemory(tex));
				}
				else
				{
					return sprites.load(name);
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
					const Image * img;
					return
						(textures.load(name)) &&
						(img = images.get(file)) &&
						(textures.get(name)->loadFromImage(*img));
				}
				else
				{
					return textures.load(name);
				}
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}