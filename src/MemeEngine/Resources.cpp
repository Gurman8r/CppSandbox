#include <MemeEngine/Resources.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Resources::Resources()
	{
	}

	Resources::~Resources()
	{
		cleanupAll();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t Resources::cleanupAll()
	{
		return
			sounds.clean() +
			effects.clean() +
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

	size_t Resources::reloadAll()
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
			effects.reload() +
			plugins.reload();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Resources::cleanup()
	{
		return cleanupAll();
	}

	bool Resources::loadFromFile(const String & filename)
	{
		SStream file;
		return ML_FileSystem.getFileContents(filename, file) && parseFile(file);
	}

	void Resources::serialize(std::ostream & out) const
	{
		for (auto item : m_manifest)
		{
			for (auto pair : item)
			{
				out << "[" << pair.first << "] \'" << pair.second << "\'"
					<< endl;
			}
			out << endl;
		}
	}

	void Resources::deserialize(std::istream & in)
	{
		parseFile((SStream &)in);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Resources::parseFile(SStream & file)
	{
		size_t count = 0;
		String line;
		while (std::getline(file, line))
		{
			if (line.trim().front() == '#')
				continue;

			if (line.find("<item>") != String::npos)
			{
				m_manifest.push_back(ManifestItem());

				while (std::getline(file, line))
				{
					line.replaceAll("$(Configuration)", Debug::configuration());
					line.replaceAll("$(PlatformTarget)", Debug::platformTarget());

					if (line.find("</item>") != String::npos)
					{
						if (parseItem(m_manifest.back()))
						{
							count++;
						}
					}
					else
					{
						size_t i;
						if ((i = line.find_first_of(":")) != String::npos)
						{
							if (const String key = String(line.substr(0, i)).trim())
							{
								if (const String val = String(
									line.substr((i + 1), (line.size() - i - 2))).trim())
								{
									m_manifest.back()[key] = val;
								}
							}
						}
					}
				}
			}
		}
		return (bool)(count);
	}

	bool Resources::parseItem(const ManifestItem & item)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto getStr = [](const HashMap<String, String> & data, const String & find)
		{
			HashMap<String, String>::const_iterator it;
			return (((it = data.find(find)) != data.end()) ? it->second : String());
		};

		/* * * * * * * * * * * * * * * * * * * * */

		const String type = item.getStr("type");
		const String name = item.getStr("name");
		if (type && name)
		{
			// Manifests
			/* * * * * * * * * * * * * * * * * * * * */
			if (type == "manifest")
			{
				return loadFromFile(name);
			}
			// Effects
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "effect")
			{
				const String m = item.getStr("model");
				const String s = item.getStr("shader");
				const int32_t w = item.getInt("width", 1920);
				const int32_t h = item.getInt("height", 1080);
				if (m && s)
				{
					Effect * e;
					return
						(e = effects.load(name)) &&
						(e->create({ w, h }, GL::ColorAttachment0)) &&
						(e->setModel(models.get(m))) &&
						(e->setShader(shaders.get(s)));
				}
				else
				{
					return effects.load(name);
				}
			}
			// Fonts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "font")
			{
				if (const String file = item.getStr("file"))
				{
					return fonts.load(name, file);
				}
				else
				{
					return fonts.load(name);
				}
			}
			// Images
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "image")
			{
				if (const String file = item.getStr("file"))
				{
					return images.load(name, file);
				}
				else
				{
					return images.load(name);
				}
			}
			// Materials
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "material")
			{
				if (const String file = item.getStr("file"))
				{
					return mats.load(name, file);
				}
				else
				{
					return mats.load(name);
				}
			}
			// Meshes
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "mesh")
			{
				if (const String file = item.getStr("file"))
				{
					return meshes.load(name, file);
				}
				else
				{
					return meshes.load(name);
				}
			}
			// Models
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "model")
			{
				if (const String file = item.getStr("file"))
				{
					return models.load(name, file);
				}
				else if (const String file = item.getStr("mesh"))
				{
					const Mesh * temp;
					return
						(models.load(name)) &&
						(temp = meshes.get(file)) &&
						(models.get(name)->loadFromMemory(*temp));
				}
				else
				{
					return models.load(name);
				}
			}
			// Plugins
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "plugin")
			{
				if (const String file = item.getStr("file"))
				{
					return plugins.load(name, file);
				}
				else
				{
					return plugins.load(name);
				}
			}
			// Scripts
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "script")
			{
				if (const String file = item.getStr("file"))
				{
					return scripts.load(name, file);
				}
				else
				{
					return scripts.load(name);
				}
			}
			// Shaders
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "shader")
			{
				if (const String file = item.getStr("file"))
				{
					return shaders.load(name, file);
				}
				else
				{
					const String vert = item.getStr("vert");
					const String geom = item.getStr("geom");
					const String frag = item.getStr("frag");
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
			// Skyboxes
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "skybox")
			{
				if (const String file = item.getStr("file"))
				{
					return skyboxes.load(name, file);
				}
				else
				{
					return skyboxes.load(name);
				}
			}
			// Sounds
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sound")
			{
				if (const String file = item.getStr("file"))
				{
					return sounds.load(name, file);
				}
				else
				{
					return sounds.load(name);
				}
			}
			// Sprites
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "sprite")
			{
				if (const String file = item.getStr("texture"))
				{
					const Texture * temp;
					return
						(sprites.load(name)) &&
						(temp = textures.get(file)) &&
						(sprites.get(name)->loadFromMemory(temp));
				}
				else
				{
					return sprites.load(name);
				}
			}
			// Textures
			/* * * * * * * * * * * * * * * * * * * * */
			else if (type == "texture")
			{
				if (const String file = item.getStr("file"))
				{
					return textures.load(name, file);
				}
				else if (const String file = item.getStr("image"))
				{
					const Image * temp;
					return
						(textures.load(name)) &&
						(temp = images.get(file)) &&
						(textures.get(name)->loadFromImage(*temp));
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