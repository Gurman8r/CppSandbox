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
		sprites.clear();
		skyboxes.clear();
		textures.clear();
		images.clear();
		fonts.clear();
	}

	bool ResourceManager::loadManifest(const Manifest & value)
	{
		for (const pair_type & p : value.getFiles())
		{
			if (!loadData(p.first, value.getPath(), p.second))
			{
				return false;
			}
		}
		return true;
	}

	int32_t ResourceManager::loadData(const String & type, const String & path, const HashMap<String, String>& files)
	{
		if (type.empty())
		{
			return ML_WARNING;
		}
		else if (type == "fonts" && (!fonts.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "images" && (!images.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "meshes" && (!meshes.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "models" && (!models.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "shaders" && (!shaders.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "skyboxes" && (!skyboxes.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "sounds" && (!sounds.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "sprites" && (!sprites.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		else if (type == "textures" && (!textures.load(files, path)))
		{
			return Debug::logError("Failed loading {0}", type);
		}
		return ML_SUCCESS;
	}
}