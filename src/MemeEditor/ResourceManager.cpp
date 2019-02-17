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
			else if (type == "fonts" && (!fonts.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "images" && (!images.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "meshes" && (!meshes.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "models" && (!models.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "shaders" && (!shaders.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "sounds" && (!sounds.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "sprites" && (!sprites.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
			else if (type == "textures" && (!textures.load(p.second, value.getPath())))
			{
				return Debug::logError("Failed loading {0}", type);
			}
		}
		return true;

		/* * * * * * * * * * * * * * * * * * * * */
	}
}