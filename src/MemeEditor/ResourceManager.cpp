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

	bool ResourceManager::loadManifest(const ResourceManifest & value)
	{
		for (const type_pair & p : value.getDataMap())
		{
			if (!loadManifestData(p.first, value.getPath(), p.second))
			{
				return Debug::logError("Failed Loading ResourceManifest");
			}
		}
		return true;
	}

	bool ResourceManager::loadManifestData(const String & type, const String & path, const file_map & files)
	{
		if (type.empty())
		{
			return true;
		}
		else if (type == "fonts"	&& !loadData(fonts,		path, files))	{ return false; }
		else if (type == "images"	&& !loadData(images,	path, files))	{ return false; }
		else if (type == "mats"		&& !loadData(mats,		path, files))	{ return false; }
		else if (type == "meshes"	&& !loadData(meshes,	path, files))	{ return false; }
		else if (type == "models"	&& !loadData(models,	path, files))	{ return false; }
		else if (type == "plugins"	&& !loadData(models,	path, files))	{ return false; }
		else if (type == "scripts"	&& !loadData(scripts,	path, files))	{ return false; }
		else if (type == "shaders"	&& !loadData(shaders,	path, files))	{ return false; }
		else if (type == "skyboxes" && !loadData(skyboxes,	path, files))	{ return false; }
		else if (type == "sounds"	&& !loadData(sounds,	path, files))	{ return false; }
		else if (type == "sprites"	&& !loadData(sprites,	path, files))	{ return false; }
		else if (type == "textures" && !loadData(textures,	path, files))	{ return false; }
		
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}