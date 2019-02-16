#include <MemeEditor/ResourceManager.hpp>
#include <MemeCore/Debug.hpp>

namespace ml
{
	using pair_type = Pair<String, HashMap<String, String>>;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager() 
	{
	}

	void ResourceManager::clean()
	{
		sounds.clear();
		sprites.clear();
		models.clear();
		meshes.clear();
		shaders.clear();
		textures.clear();
		images.clear();
		fonts.clear();
	}

	bool ResourceManager::loadManifest(const Manifest & value)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		for (const pair_type & p : value.getFiles())
		{
			const String & type = p.first;

			if (type.empty())
			{
				continue;
			}
			else if (type == "font" && (!fonts.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "image" && (!images.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "mesh" && (!meshes.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "model" && (!models.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "shader" && (!shaders.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "sound" && (!sounds.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "sprite" && (!sprites.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "texture" && (!textures.load(value.getPath(), p.second)))
			{
				return Debug::logError("Failed loading {0}", type);
			}
		}
		return true;

		/* * * * * * * * * * * * * * * * * * * * */
	}
}